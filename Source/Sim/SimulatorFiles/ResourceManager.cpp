#include "ResourceManager.h"
using RM = ResourceManager;

RM::ResourceRemaining RM::getResourceRemainingData(
    const Resource& res,
    bool considerStatus,
    Task::TaskStatus tStatus
)
{
    RM::ResourceRemaining resRem;
    // Firstly, initialise the general resource parameters
    // and doesn't include part that are occupied
    resRem.discSize = res.resDesc.discSize;
    resRem.memSize = res.resDesc.memSize;
    resRem.procCount = res.resDesc.procCount;

    // Subtracts from resource parameters the paramets
    // that occupied by each task
    for (const auto& task : res.performingTasks)
    {
        // If it is need to include task status and
        // he isn't coincide, the skip it
        if (considerStatus && task->status != tStatus)
            continue;
        // Multiply each parameter with the number of tasks with
        // subtasks, which are performing at the moment
        resRem.discSize -= task->resDesc.discSize * task->count;
        resRem.memSize -= task->resDesc.memSize * task->count;
        resRem.procCount -= task->resDesc.procCount * task->count;
    }

    return resRem;
}

int RM::howManyTasksCanBePerformed(const Task& task, const Resource& res)
{
    int count = 0;
    // Get information about all available resource's parammeters
    RM::ResourceRemaining resRem = RM::getResourceRemainingData(res);
    while (true)
    {
        // Subtracts from resource's parameters the parameters,
        // which is nedded for performing 1 task
        resRem.discSize -= task.resDesc.discSize;
        resRem.memSize -= task.resDesc.memSize;
        resRem.procCount -= task.resDesc.procCount;

        // If at least 1 of the parameters is smaller than 0, and
        // it defines that there are a small number of components
        // for performing this task, and for reason leave the loop
        // and return result
        if (resRem.discSize < 0 || resRem.memSize < 0 || resRem.procCount < 0)
            break;
        // If there is an enough number of components, increase the result's amount
        ++count;
    }
    return count;
}

double RM::getMinTimeToFree(const Resource& res)
{
    // If there are no tasks on the resource, return 0
    if (res.performingTasks.empty())
        return 0;
    // Find the smallest performTime from all tasks
    double minTimeToFree = res.performingTasks[0]->performTime;
    for (const Task* task : res.performingTasks)
    {
        if (task->performTime < minTimeToFree)
            minTimeToFree = task->performTime;
    }
    return minTimeToFree;
}

Resource* RM::findAnyFreeResource(
    const Task& task,
    const vector<Resource*>& resources,
    bool areSubTasksConnected
)
{
    for (Resource* resource : resources)
    {
        // If tasks aren't connected to each other and at least 1 task
        // can be performed, then return the resource
        if (!areSubTasksConnected &&
            ResourceManager::howManyTasksCanBePerformed(task, *resource) > 0
            ) return resource;
        // If tasks are connected to each other, the resource should be
        // able to perform all tasks at once
        else if (areSubTasksConnected &&
            ResourceManager::howManyTasksCanBePerformed(task, *resource) >= task.count
            ) return resource;
    }
    return nullptr;
}

bool RM::canTaskBeSentToResource(
    const Task& task,
    const Resource& res,
    bool areSubTasksConnected,
    int* outSubTasksCount
)
{
    bool canTaskBeSent = false;
    int count = ResourceManager::howManyTasksCanBePerformed(task, res);

    // If tasks are connected, the resource needs to be free,
    // to tasks can fully be performed at once
    if (areSubTasksConnected)
    {
        if (count >= task.count)
            canTaskBeSent = true;
    }
    // Otherwise for perform, it is enough any number of tasks on the
    // resource
    else
    {
        if (count > 0)
            canTaskBeSent = true;
    }
    // If the count increases, than general number of tasks, then
    // will set for count the value of the number of tasks
    count = (count > task.count) ? task.count : count;

    // Check for nullptr
    if (outSubTasksCount)
        *outSubTasksCount = count;

    return canTaskBeSent;
}

double RM::calcResourceStagnation(vector<Resource>& resources)
{
    double busyProcessors = 0;
    double allProcessors = 0;
    for (const Resource& res : resources)
    {
        allProcessors += res.resDesc.procCount;
        busyProcessors += RM::getResourceRemainingData(res).procCount;
    }
    if (allProcessors == 0)
        return 0;
    return busyProcessors / allProcessors * 100;
}

const Resource* RM::findResourceTaskIsPerfOn(
    const Task* task,
    const vector<Resource>& resources
)
{
    for (const Resource& res : resources)
    {
        for (const Task* taskOnRes : res.performingTasks)
        {
            if (task == taskOnRes)
                return &res;
        }
    }
    return nullptr;
}
