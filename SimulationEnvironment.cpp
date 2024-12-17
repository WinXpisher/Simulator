#include <algorithm> // ��� std::min_element
#include "SimulationEnvironment.h"
#include "ResourceManager.h"

using DM = DistributionMethod;
SimulationEnvironment::SimulationEnvironment(DataBase* dataBase):
    dataBase(dataBase), taskAnalizer(dataBase)
{
    initDMethods();
}

SimulationEnvironment::~SimulationEnvironment()
{
    for (DM* dmPtr : distributionMethods)
    {
        delete dmPtr;
    }
}

void SimulationEnvironment::initDMethods()
{
    for (auto dm : DM::dMethods)
    {
        switch (dm)
        {
        case DM::FCFS:
            distributionMethods.push_back(new FCFS());
            break;
        case DM::LIFO:
            distributionMethods.push_back(new LIFO());
            break;
        case DM::HPF:
            distributionMethods.push_back(new HPF());
            break;
        case DM::BACKFILL:
            break;
        case DM::SIMPLEX:
            distributionMethods.push_back(new SIMPLEX());
            break;
        case DM::SMART:
            break;
        default:
            break;
        }
    }
}

void SimulationEnvironment::runSimulation()
{
    taskAnalizer.analizeAllTasks();
    simResults.clear();
    // ��������� �� ������� ������ �����������
    for (DM* dm : distributionMethods)
    {
        SimulationResult simRes = runSimulationForDMethod(dm);
        simResults.push_back(simRes);
    }
}

const vector<SimulationEnvironment::SimulationResult>&
    SimulationEnvironment::getResults() const
{
    return simResults;
}

SimulationEnvironment::SimulationResult SimulationEnvironment::runSimulationForDMethod(
    const DM* dm
)
{
    SimulationResult simResult {dm->getId(), 0, 0, 0};
    // ������� ��������� (��������������� ��� ���������� ���������� ���� ����������)
    int waitingCount = 0;
    double waitingTime = 0; // ��������� ��� ����������
    // �������� ���������� ������ �����, �� �� ����� ������� CANCELLED
    vector<TaskAnalizer::AnalizerResult> anResultsCopy = 
        taskAnalizer.getAnalizeResultClear();
    TaskAnalizer::AnalizerResult anResult;
    Resource* resource=nullptr;
    while (dm->nextTask(anResultsCopy, anResult, resource))
    {
        // ���� ����� �������� �� �������� ������
        if (resource == nullptr)
        {
            while (true)
            {
                // ������� �� ��������� ����-����� �������, ���� ���������� ���������
                waitingTime += waitForRelease(anResult);
                ++waitingCount;
                // ��������� ����� ������
                resource = findAnyFreeResource(anResult.task, anResult.resources);
                // ��������� �������� �� ���������, ����� ����� ���� task.count,
                // ��� �������� ������ ����� �� ���������� ����������
                simResult.execTime += sendTaskToResource(anResult.task, *resource);
                // ���� �������� ������� ����������, ��������� ����
                if (anResult.task.count <= 0)
                    break;
            }
        }
        else
        {
            while (true)
            {
                // ������� �� ��������� �������� �������
                waitingTime += waitForRelease(anResult, resource);
                ++waitingCount;
                // ��������� �������� �� ���������, ����� ����� ���� task.count,
                // ��� �������� ������ ����� �� ���������� ����������
                simResult.execTime += sendTaskToResource(anResult.task, *resource);
                // ���� �������� ������� ����������, ��������� ����
                if (anResult.task.count <= 0)
                    break;
            }
        }
        resource = nullptr;
    }
    // ������ �� ��������� ��� �����
    simResult.execTime += waitForPerformAllTasks(dataBase->availableResources);
    // ������ �� ���������� ���� ��������� ��������� ��� ����������
    simResult.execTime += waitingTime;
    // ����������� ������ �������� ����������
    simResult.avWaitTime = waitingTime / waitingCount;
    return simResult;
}

Resource* SimulationEnvironment::findAnyFreeResource(
    const Task& task,
    const vector<Resource*>& resources
)
{
    bool areSubTasksConnected = taskAnalizer.areSubTasksConnected(task);
    for (Resource* resource : resources)
    {
        // ���� ������ �� ��'���� �� ����� � ���� � ���� ������ ����
        // ����������,��������� ��� ������
        if (!areSubTasksConnected &&
            ResourceManager::howManyTasksCanBePerformed(task, *resource) > 0
            ) return resource;
        // ���� ������ ��'���� �� ����� ������ ������� ����
        // ����� �������� �� �� ������
        else if (areSubTasksConnected &&
            ResourceManager::howManyTasksCanBePerformed(task, *resource) >= task.count
            ) return resource;
    }
    return nullptr;
}

double SimulationEnvironment::waitForPerformAllTasks(vector<Resource>& resources)
{
    double maxTimeToWait = 0; // ���, �������� ��� ��������� ��� �������
    // ��������� �� ������� �������
    for (Resource& res : resources)
    {
        // ��������� �� ������� ��������, �� ���������� �� ��������� ������
        for (Task& task : res.performingTasks)
        {
            // ������ �������� �������� ��� ������, �� �� ��������
            // ������ �������
            if (task.performTime > maxTimeToWait)
                maxTimeToWait = task.performTime;
        }
    }
    waitForTime(maxTimeToWait, resources); // ��������� ����������
    return maxTimeToWait; // ��������� ��� ����������
}

double SimulationEnvironment::waitForRelease(
    TaskAnalizer::AnalizerResult anResult,
    Resource* resourcePtr
)
{
    double waitingTime; // ��� ����������
    // ���� ������ ������, ������ �� ���� ���������
    if (resourcePtr != nullptr)
    {
        waitingTime = waitForReleaseParticular(
            anResult.task,
            *resourcePtr
        );
    }
    // ������ ������� ��� ����� ������, ���� ���������� ���������
    else
    {
        waitingTime = waitForReleaseAvailable(anResult);
    }
    return waitingTime;
}

double SimulationEnvironment::waitForReleaseAvailable(
    TaskAnalizer::AnalizerResult anResult
)
{
    // �� � ������ ��'�������
    bool areSubTasksConnected = taskAnalizer.areSubTasksConnected(anResult.task);
    //cout << "canBePerformedSimple: " << taskAnalizer.canBePerformedConnected(
    //    anResult.task, *anResult.resources[0]
    //) << endl;
    double waitingTime = 0; // ��� ����������
    double minTimeToWait;
    while (true)
    {
        // ���������� �� � ������ ������ ���������
        bool resourceAvailable = std::any_of(
            anResult.resources.begin(),
            anResult.resources.end(),
            [anResult, areSubTasksConnected](const Resource* resource) {
                // ���� ������ ���'����, �� ������ ����� �������� ���,
                // ��� �������� ����� ������� ���������� �� ���� ���
                if (areSubTasksConnected)
                    return ResourceManager::howManyTasksCanBePerformed(
                        anResult.task,
                        *resource
                    ) >= anResult.task.count;
                // ������ ��������� ��������� ��������� ����-��� �������
                // ����� �� ������
                else
                    return ResourceManager::howManyTasksCanBePerformed(
                        anResult.task,
                        *resource
                    ) > 0;
            }
        );

        // ���� ���, �������� � �����
        if (resourceAvailable)
            break;

        // ������ ������ ������, ���� ��������� �� ��� ������ ����� ������
        auto minIter = std::min_element(
            anResult.resources.begin(),
            anResult.resources.end(),
            [](const Resource* r1, const Resource* r2) {
                return ResourceManager::getMinTimeToFree(*r1) <
                    ResourceManager::getMinTimeToFree(*r2);
            }
        );
        Resource* minResource = *minIter;
        // �������� ��������� ��� ��� ��������� �������
        minTimeToWait = ResourceManager::getMinTimeToFree(*minResource);
        // ��������� ����������
        waitForTime(minTimeToWait, dataBase->availableResources);
        waitingTime += minTimeToWait;
    }
    return waitingTime;
}

double SimulationEnvironment::waitForReleaseParticular(
    const Task& task,
    Resource& resource
)
{
    // �� � ������ ��'�������
    bool areSubTasksConnected = taskAnalizer.areSubTasksConnected(task);
    double waitingTime = 0; // ��� ����������
    double minTimeToWait;
    // ���� ������ ���'����, ���� ������������ �� ��� ��, ���� �������
    // ������� �� ���� ��������� ��� ��������� ������ ��������, ������ -
    // ��������� ��������� ��������� ����-��� ������� ����� �� ��������
    while (
        (areSubTasksConnected &&
            ResourceManager::howManyTasksCanBePerformed(task, resource) < task.count) ||
        (!areSubTasksConnected && 
            ResourceManager::howManyTasksCanBePerformed(task, resource) == 0
        ))
    {
        // ������ ��������� �������, ���� ���������� �������� ����
        minTimeToWait = ResourceManager::getMinTimeToFree(resource);
        waitForTime(minTimeToWait, dataBase->availableResources);
        waitingTime += minTimeToWait;
    }
    return waitingTime;
}

void SimulationEnvironment::waitForTime(
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
            iter->performTime -= time; // ������� ���, ���� ��� ������
            // ���� �������� ��� ���� ��������, ��������� ����
            if (iter->performTime <= 0)
                iter = res.performingTasks.erase(iter);
            else
                ++iter;
        }
    }
}

void SimulationEnvironment::freeAllResources()
{
    for (Resource& resource : dataBase->availableResources)
        resource.performingTasks.clear();
}

double SimulationEnvironment::sendTaskToResource(
    Task& task,
    Resource& resource
)
{
    double sentTime = 0; // �������� ����������
    // ������ ����� ����� ���� ���������� �� ������
    int canBePerformed = ResourceManager::howManyTasksCanBePerformed(
        task,
        resource
    );
    // ������ ����� ���� ��������
    int willBePerformed = (canBePerformed > task.count)?
        task.count : canBePerformed;

    // ��������� ���� ��������, ��� ���� ���������� � ������
    Task taskCopy = task;
    // ���������� ���� �� ����'������ ��� ������, � ����� �� 
    // ������� �����, ��� ��� � ������ ���������� ����
    taskCopy.count = willBePerformed;

    // ������ �������� �� ������� �������, �� ����� ����������� �� ������
    resource.performingTasks.push_back(taskCopy);
    // �������� ������� �����, �� �� ���������� ����������
    task.count -= willBePerformed;
    // ��������� �������� �������� ��� ��������� ����� �� ��������
    return getNetworkDelay(task, resource, willBePerformed);
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