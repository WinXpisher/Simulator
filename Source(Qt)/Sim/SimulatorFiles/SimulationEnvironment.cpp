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

    // Get the result of analysing tasks, which haven't CANCELLED status
    simContext.anResults =
        taskAnalizer.getAnalizeResultClear();

    //simContext.assignedResource = nullptr;
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
            
            // If there are first iteration
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
                // To output task, which was cancelled
                for (const Task& task : dataBase->tasks)
                {
                    if (task.status == Task::TaskStatus::CANCELLED)
                        logger->logTaskCancelled(task);
                }
            }

            simContext.actionTaken = false;
            // If there are have some task for sending
            if (simContext.subTasksRemain > 0)
            {
                // Use intended resource, if it exists, otherwise
                // searching for available
                Resource* targetResource = dm->nextResource(simContext.anResult);
                if (!targetResource)
                {
                    targetResource = ResourceManager::findAnyFreeResource(
                        *simContext.anResult.task,
                        simContext.anResult.resources,
                        simContext.areSubTasksConnected
                    );
                }

                // If a resource was found, then send the task (ot its part)
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
                }
            }
            // If it doesn't have any tasks, then load new task
            else if (simContext.hasTask)
            {
                simContext.hasTask = dm->nextTask(
                    simContext.anResults,
                    simContext.anResult
                );
                // If task was got
                if (simContext.hasTask)
                {
                    simContext.subTasksRemain = simContext.anResult.task->count;
                    simContext.areSubTasksConnected = 
                        taskAnalizer.areSubTasksConnected(*simContext.anResult.task);
                    simContext.actionTaken = true;
                }
            }
            // Check whether all tasks are performed
            if (simContext.subTasksRemain <= 0 && !simContext.hasTask)
            {
                if (haveAllTasksPerformed())
                {
                    break;
                }
            }
        }
        // If there aren't any actions performed, then waiting
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
    // Simulate zero waiting, to tasks with 0 sending time and (or) 0 performing time
    // are processed at once. And a calling order differs from modelWaiting in method.
    waitForSendingPool(0);
    waitForResources(0, dataBase->availableResources);

    for (const Task& task : dataBase->tasks)
    {
        // If at least 1 task doesn't have performed or cancelled status,
        // then there are the task, which will be performed
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
    // The number of tasks, which are sent
    int sentSubTasksCount = 0;
    // Get task copy, but if task has DIVIDED status, then includes its
    // child elements, and it is from general sum of subtasks of current task
    // subtract sum of child's tasks.
    // Otherwise copy doesn't change.
    Task copyWithRemainingCount = getTaskCopyWithRemainingCount(task);
    bool canTaskBeSent = ResourceManager::canTaskBeSentToResource(
        copyWithRemainingCount, resource, areSubTasksConnected, &sentSubTasksCount
    );

    if (canTaskBeSent)
    {
        // Task, which will be sent
        Task* taskToBeSent;
        // If the task will be sent particularly, and some part or task
        // are distributed, then take his part, create on one part's base
        // a new task, which will be a child
        if (sentSubTasksCount < task.count || task.status == Task::TaskStatus::DIVIDED)
        {
            // Create copy of main task, which will be child
            Task childTask = task;
            childTask.simulationInfo.childTasks = std::list<Task>();
            // Give id in followed format [parentId].[childId], for example 1.1
            childTask.id += "." + std::to_string(
                task.simulationInfo.childTasks.size() + 1
            );

            childTask.simulationInfo.parentTaskPtr = &task;
            childTask.count = sentSubTasksCount;

            task.simulationInfo.childTasks.push_back(childTask);
            taskToBeSent = &task.simulationInfo.childTasks.back();

            // Set DIVIDED or DIVIDED_RUNNING status to the parent task
            setDividedTaskStatus(task);
        }
        else
        {
            // Otherwise will send full task
            taskToBeSent = &task;
        }

        // Although the task isn't performing yet, add it to the resource,
        // to mark that the resource is already occupied, at least its status
        // will be SENDING
        taskToBeSent->status = Task::TaskStatus::SENDING;
        resource.performingTasks.push_back(taskToBeSent);

        logger->logTaskSendingPool(*taskToBeSent);

        // Send task to the tasks' pool to the waiting
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
    // How many undistributed tasks are left, including child tasks
    int remainingCount = Task::getRemainingSubTasksCount(parentTask);
    // If there are no tasks
    if (!remainingCount)
        parentTask.status = Task::TaskStatus::DIVIDED_RUNNING;
    // Otherwise parent task will be marked as DIVIDED
    else
        parentTask.status = Task::TaskStatus::DIVIDED;
}

void SimulationEnvironment::wait(int time)
{
    // Simulate the waiting
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

    // Firstly simulate the waiting on the resources, which are running,
    // to tasks cannot be sending and running at once on the some part
    waitForResources(time, dataBase->availableResources);
    waitForSendingPool(time);
}

void SimulationEnvironment::modelWaitingForSubTasks(double time)
{
    for (Task& task : dataBase->tasks)
    {
        if (task.status == Task::TaskStatus::WAITING)
        {
            // Add product of performing time for 1 task and number of subtasks
            // on the one task
            task.simulationInfo.waitingTime += time *
                task.count;
        }
        else if (task.status == Task::TaskStatus::DIVIDED)
        {
            // Add product of performing time of 1 task and number of
            // tasks, which aren't performed yet
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

    // Process number of tasks at once, that doesn't exceed channelCount
    int tasksToProcess = std::min(static_cast<int>(sendingPool.size()), channelCount);

    // Itearte through tasks, which can be processed at once
    for (int i = 0; i < tasksToProcess; i++)
    {
        sendingPool[i].timeToSend -= time;

        // If task are sent
        if (sendingPool[i].timeToSend <= 0)
        {
            // Set RUNNING status
            sendingPool[i].task->status = Task::TaskStatus::RUNNING;

            // To log, that the task are snet on the resource
            const Resource* resToLog =
                ResourceManager::findResourceTaskIsPerfOn(
                    sendingPool[i].task,
                    dataBase->availableResources
                );
            if (resToLog)
                logger->logTaskSentToRes(*sendingPool[i].task, *resToLog);
        }
    }

    // Delete all undone tasks from the beginning of pool into tasksToProcess
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
    // Iterate through every resource
    for (Resource& res : resources)
    {
        // Iterate through each task, taht are performing on current resource.
        // Use iterator, to delete task, which has remaning time to the end of
        // performing, smaller or equal 0
        for (
            auto iter = res.performingTasks.begin();
            iter != res.performingTasks.end();
            )
        {
            // If task are running
            if ((*iter)->status == Task::TaskStatus::RUNNING)
                // Add time, which will be gone
                (*iter)->simulationInfo.timePerformed += time;

            // If task was performed
            if ((*iter)->performTime <= (*iter)->simulationInfo.timePerformed)
            {
                // Actual value of performing time can exceed the general
                // time of performing, will guarantee that they are equal
                (*iter)->simulationInfo.timePerformed = (*iter)->performTime;
                // Mark task as performed
                (*iter)->status = Task::TaskStatus::PERFORMED;
                logger->logTaskPerformed(**iter);
                finishDividedTaskIfNeed(**iter);
                // Delete task from the list, which are performing
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
    // If parentPtr=nullptr, it is mean taht the task aren't child
    if (!parentPtr)
        return;

    // If tasks aren't performing (or performing particularly)
    // in divided mode, they cannot be performed
    if (parentPtr->status != Task::TaskStatus::DIVIDED_RUNNING)
        return;

    for (Task& child : parentPtr->simulationInfo.childTasks)
    {
        // If at least 1 child task isn't performed,
        // parent task couldn't be performed
        if (child.status != Task::TaskStatus::PERFORMED)
            return;
    }
    // If all tests are passed, parent task was performed
    parentPtr->status = Task::TaskStatus::PERFORMED;
    logger->logTaskPerformed(*parentPtr);
}


double SimulationEnvironment::getNetworkDelay(
    const Task& task,
    const Resource& res,
    int subTasksCount
)
{
    // Divide size of task and channel's bandwidth size/(size/time)
    // and get time, for which 1 task is sent
    double delayPerSubTask = task.subTaskSize / res.bandwidth;
    return delayPerSubTask * subTasksCount;
}
