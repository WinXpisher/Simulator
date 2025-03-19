#include <chrono>
#include <thread>
#include "SimulationEnvironment.h"
#include "ResourceManager.h"

using DM = DistributionMethod;
SimulationEnvironment::SimulationEnvironment(DataBase* dataBase, Logger* logger):
    logger(logger), dataBase(dataBase), taskAnalizer(dataBase)
{
    
}

SimulationEnvironment::~SimulationEnvironment()
{
    for (DM* dmPtr : distributionMethods)
    {
        delete dmPtr;
    }
}

void SimulationEnvironment::prepareForSimulation(
    int channelCount,
    int timeUnitToSkip,
    int waitMilliSec
)
{
    this->channelCount = channelCount;
    simContext.timeUnitToSkip = timeUnitToSkip;
    simContext.waitMilliSec = waitMilliSec;

    initTasksSimulationInfo();
    initSimContext();
}

void SimulationEnvironment::initSimContext()
{
    subTaskWaitingTimeSum = 0;
    resourceStagnationSum = 0;
    simulationClock = 0;

    taskAnalizer.analizeAllTasks();

    // �������� ���������� ������ �����, �� �� ����� ������� CANCELLED
    simContext.anResults =
        taskAnalizer.getAnalizeResultClear();

    simContext.assignedResource = nullptr;     
    simContext.hasTask = true;                 
    simContext.subTasksRemain = 0;
    simContext.areSubTasksConnected = false;
}

void SimulationEnvironment::initTasksSimulationInfo()
{
    for (Task& task : dataBase->tasks)
    {
        task.simulationInfo.parentTaskPtr = nullptr;
        task.simulationInfo.timePerformed = 0;
        task.simulationInfo.waitingTime = 0;
    }
}

void SimulationEnvironment::runSimulation(const DM* dm)
{
    bool isItFirstIteration = true;
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(dataBaseMutex);
            
            // ���� �� ����� ��������
            if (isItFirstIteration)
            {
                isItFirstIteration = false;
                logger->logSimulationData(
                    SimulationData
                    {
                        simulationClock,
                        getSubTaskWaitingTimeAv(),
                        getResourceStagnationAv()
                    }
                );
                // �������� ��������, �� ���� ���������
                for (const Task& task : dataBase->tasks)
                {
                    if (task.status == Task::TaskStatus::CANCELLED)
                        logger->logTaskCancelled(task);
                }
            }

            simContext.actionTaken = false;
            // ���� �� � ������ ��� ��������
            if (simContext.subTasksRemain > 0)
            {
                // ������������� ����������� ������, ���� �� �, ������ ������ ������
                Resource* targetResource = simContext.assignedResource;
                if (!targetResource)
                {
                    targetResource = ResourceManager::findAnyFreeResource(
                        *simContext.anResult.task,
                        simContext.anResult.resources,
                        simContext.areSubTasksConnected
                    );
                }

                // ���� ������ ��������, ����������� �������� (��� ���� �������)
                if (targetResource)
                {
                    int sent = trySendTaskToResource(
                        *simContext.anResult.task,
                        *targetResource,
                        simContext.areSubTasksConnected
                    );
                    if (sent > 0)
                    {
                        simContext.subTasksRemain -= sent;
                        simContext.actionTaken = true;
                    }
                    // ���� �� �� ����������� ������, ������� ���� ��� 
                    // ���������� ������
                    if (targetResource != simContext.assignedResource)
                    {
                        targetResource = nullptr;
                    }
                }
            }
            // ���� � ����� ��� �������� ����, ����������� ���� ��������
            else if (simContext.hasTask)
            {
                Resource* resource = nullptr;
                simContext.hasTask = dm->nextTask(
                    simContext.anResults,
                    simContext.anResult,
                    resource
                );
                // ���� �������� ���� ��������
                if (simContext.hasTask)
                {
                    simContext.subTasksRemain = simContext.anResult.task->count;
                    simContext.areSubTasksConnected = 
                        taskAnalizer.areSubTasksConnected(*simContext.anResult.task);
                    // �������� ������, ���� �� �������
                    simContext.assignedResource = resource;
                    simContext.actionTaken = true;
                }
            }
            if (haveAllTasksPerformed())
            {
                break;
            }
            // ����������, �� �� �������� ��������
            if (simContext.subTasksRemain <= 0 && !simContext.hasTask)
            {
                if (haveAllTasksPerformed())
                {
                    break;
                }
            }
        }
        // ���� ����� 䳿 �� ��������, ������
        if (!simContext.actionTaken)
        {
            {
                std::lock_guard<std::mutex> lock(dataBaseMutex);
                simulationClock += simContext.timeUnitToSkip;
                modelWaiting(simContext.timeUnitToSkip);
            }
            wait(simContext.waitMilliSec);
        }
    }
}

bool SimulationEnvironment::haveAllTasksPerformed()
{
    // ��������� ������� ����������, ��� ������ � �������� ����� ����������
    // �� (���) �������� ����� ��������� ���������� ������. ������� �������
    // ������� ��������� �� ����, ���� � ����� modelWaiting.
    waitForSendingPool(0);
    waitForResources(0, dataBase->availableResources);

    for (const Task& task : dataBase->tasks)
    {
        // ���� ��� ���� �������� �� �� ������ �������� ��� ���������,
        // ������� �� � ��������, �� ������ ������������
        if (task.status != Task::TaskStatus::PERFORMED &&
            task.status != Task::TaskStatus::CANCELLED)
        {
            return false;
        }
    }
    return true;
}


int SimulationEnvironment::trySendTaskToResource(
    Task& task,
    Resource& resource,
    bool areSubTasksConnected
)
{
    // ������� �����, �� ���� ��������
    int sentSubTasksCount = 0;
    // �������� ���� ��������, ��� ���� �������� �� ������ DIVIDED,
    // �� ������������ ���� ������ ��������, � ���� �� �������� ���� �����
    // ��������� �������� ��������� ���� ����� ������� �������.
    // ������ ���� ��� �� ���������.
    Task copyWithRemainingCount = getTaskCopyWithRemainingCount(task);
    bool canTaskBeSent = ResourceManager::canTaskBeSentToResource(
        copyWithRemainingCount, resource, areSubTasksConnected, &sentSubTasksCount
    );

    if (canTaskBeSent)
    {
        Task* taskToBeSent; // ��������, ��� ���� ������������
        // ���� ����������� ���� �� ��� ��������, � ����� �������
        // ��� �������� ��� � ���������, �� ������ ���� �������,
        // ��������� �� �� ����� ���� ��������, ��� ����� �������
        if (sentSubTasksCount < task.count || task.status == Task::TaskStatus::DIVIDED)
        {
            // ��������� ���� ������� ������, ��� ����� ���������
            Task childTask = task;
            childTask.simulationInfo.childTasks = std::list<Task>();
            // ���������� ��� � ������ [parentId].[childId], ��������� 1.1
            childTask.id += "." + std::to_string(
                task.simulationInfo.childTasks.size() + 1
            );

            childTask.simulationInfo.parentTaskPtr = &task;
            childTask.count = sentSubTasksCount;

            task.simulationInfo.childTasks.push_back(childTask);
            taskToBeSent = &task.simulationInfo.childTasks.back();

            // ������������ ������������ �������� ������ DIVIDED ��� DIVIDED_RUNNING
            setDividedTaskStatus(task);
        }
        else
        {
            // ������ ������ ���������� ��� �������� �������
            taskToBeSent = &task;
        }

        // ���� �������� �� �� ����������, ������ ���� �� �������,
        // ��� �������, �� ������ ��� ��������, ���� ������ �� ���� SENDING
        taskToBeSent->status = Task::TaskStatus::SENDING;
        resource.performingTasks.push_back(taskToBeSent);

        logger->logTaskSendingPool(*taskToBeSent);

        // ����������� ������ � ��� ����� �� ����������
        SendingTask st {
            taskToBeSent,
            getNetworkDelay(*taskToBeSent, resource, sentSubTasksCount)
        };
  
        sendingPool.push_back(st);

        return sentSubTasksCount;
    }
    return 0;
}

Task SimulationEnvironment::getTaskCopyWithRemainingCount(const Task& task)
{
    if (task.status != Task::TaskStatus::DIVIDED)
        return task;
 
    Task copy = task;
    copy.count = Task::getRemainingSubTasksCount(task);
    return copy;
}

void SimulationEnvironment::setDividedTaskStatus(Task& parentTask)
{
    // ������ ���������� ������������� �� ������� �����, ����������
    // ������ ������� �������
    int remainingCount = Task::getRemainingSubTasksCount(parentTask);
    // ���� ����� �� ����������
    if (!remainingCount)
        parentTask.status = Task::TaskStatus::DIVIDED_RUNNING;
    // ������ ������� ���������� �������� �� ������ ��������
    else
        parentTask.status = Task::TaskStatus::DIVIDED;
}

void SimulationEnvironment::wait(int time)
{
    // ��������� ����������
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void SimulationEnvironment::modelWaiting(double time)
{
    resourceStagnationSum +=
        ResourceManager::calcResourceStagnation(dataBase->availableResources);
    modelWaitingForSubTasks(time);
    calcSubTaskWaitingTimeCurrent();

    logger->logSimulationData(
        SimulationData
        {
            simulationClock,
            getSubTaskWaitingTimeAv(),
            getResourceStagnationAv()
        }
    );

    // �������� ��������� ���������� �� ��������, �� ��� �����������,
    // ��� �������� �� ����� ����������� � ������ ���������� �� ����� �������
    waitForResources(time, dataBase->availableResources);
    waitForSendingPool(time);
}

void SimulationEnvironment::modelWaitingForSubTasks(double time)
{
    for (Task& task : dataBase->tasks)
    {
        if (task.status == Task::TaskStatus::WAITING)
        {
            // ������ ������� ���� ��������� ���� ������ �� �������
            // ����� � ������ �������
            task.simulationInfo.waitingTime += time *
                task.count;
        }
        else if (task.status == Task::TaskStatus::DIVIDED)
        {
            // ������ ������� ���� ��������� ���� ������ �� �������
            // �����, �� �� ���������� ��������
            task.simulationInfo.waitingTime += time *
                Task::getRemainingSubTasksCount(task);
        }
    }
}

void SimulationEnvironment::calcSubTaskWaitingTimeCurrent()
{
    subTaskWaitingTimeSum = 0;
    for (const Task& task : dataBase->tasks)
    {
        subTaskWaitingTimeSum += task.simulationInfo.waitingTime;
    }
}

int SimulationEnvironment::getSubTasksCount() const
{
    int sum = 0;
    for (const Task& task : dataBase->tasks)
        sum += task.count;
    return sum;
}

void SimulationEnvironment::waitForSendingPool(double time)
{
    if (sendingPool.empty())
        return;

    // ���������� ��������� ������� �������, �� �� �������� channelCount
    int tasksToProcess = std::min(static_cast<int>(sendingPool.size()), channelCount);

    // ��������� �� �������, �� ������ �������� ���������
    for (int i = 0; i < tasksToProcess; i++)
    {
        sendingPool[i].timeToSend -= time;

        // ���� �������� ���� ����������
        if (sendingPool[i].timeToSend <= 0)
        {
            // ������������ ������ RUNNING
            sendingPool[i].task->status = Task::TaskStatus::RUNNING;

            // ������, �� ������ ���������� �� ������
            const Resource* resToLog =
                ResourceManager::findResourceTaskIsPerfOn(
                    sendingPool[i].task,
                    dataBase->availableResources
                );
            if (resToLog)
                logger->logTaskSentToRes(*sendingPool[i].task, *resToLog);
        }
    }

    // ��������� �� �������� ������ � ������� ���� �� tasksToProcess
    sendingPool.erase(
        std::remove_if(sendingPool.begin(), sendingPool.begin() + tasksToProcess,
            [](const SendingTask& task) { return task.timeToSend <= 0; }),
        sendingPool.begin() + tasksToProcess);
}


void SimulationEnvironment::waitForResources(
    double time,
    vector<Resource>& resources
)
{
    // ��������� �� ������� �������
    for (Resource& res : resources)
    {
        // ��������� �� ������� ��������, �� ���������� �� ��������� ������.
        // ������������� ��������, ��� �������� ��������, � ���� ���, ��
        // ��������� �� ���� �� ���������, ����� ��� ������� ����
        for (
            auto iter = res.performingTasks.begin();
            iter != res.performingTasks.end();
            )
        {
            // ���� ������ ��� ����������
            if ((*iter)->status == Task::TaskStatus::RUNNING)
                // ������ ���, ���� ��� ������
                (*iter)->simulationInfo.timePerformed += time;

            // ���� �������� ��� ���� ��������
            if ((*iter)->performTime <= (*iter)->simulationInfo.timePerformed)
            {
                // �������� �������� ���� ��������� ����� ���������� ���������
                // ��� ���������, ���� ���������, �� ���� ���������
                (*iter)->simulationInfo.timePerformed = (*iter)->performTime;
                // ������� �������� �� ��������
                (*iter)->status = Task::TaskStatus::PERFORMED;
                logger->logTaskPerformed(**iter);
                finishDividedTaskIfNeed(**iter);
                // ��������� �������� � ������ ���, �� �����������
                iter = res.performingTasks.erase(iter);
            }
            else
                ++iter;
        }
    }
}

void SimulationEnvironment::finishDividedTaskIfNeed(Task& childTask)
{
    Task* parentPtr = childTask.simulationInfo.parentTaskPtr;
    // ���� parentPtr=nullptr, ������� �������� �� � �������
    if (!parentPtr)
        return;

    // ���� �������� �� ���������� (��� ���������� �� �������)
    // � ���������� �����, ���� �� ���� ���� ��������
    if (parentPtr->status != Task::TaskStatus::DIVIDED_RUNNING)
        return;

    for (Task& child : parentPtr->simulationInfo.childTasks)
    {
        // ���� ���� � ���� ������ �������� �� � ���������,
        // ���������� �������� �� ���� ��������� ���������
        if (child.status != Task::TaskStatus::PERFORMED)
            return;
    }
    // ���� �� �������� �������, ��������� �������� ���� ��������
    parentPtr->status = Task::TaskStatus::PERFORMED;
    logger->logTaskPerformed(*parentPtr);
}


double SimulationEnvironment::getNetworkDelay(
    const Task& task,
    const Resource& res,
    int subTasksCount
)
{
    // ����� ����� ������ �� ��������� �������� ������ size/(size/time)
    // � �������� ���, �� ���� ����������� ���� ������
    double delayPerSubTask = task.subTaskSize / res.bandwidth;
    return delayPerSubTask * subTasksCount;
}