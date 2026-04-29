#include "TaskAnalizer.h"

void TaskAnalizer::analizeAllTasks()
{
    // Call method to get results for each task,
    // and results will be recorded in conResult
    analizeConnectivity();
    for (ConnectivityResult& cr : conResult)
    {
        analizeTask(*cr.task, cr.areConnected);
    }
    // Fill vector, where result are cleaned from cancelled tasks
    clearFromCancelled();
}

void TaskAnalizer::clearFromCancelled()
{
    anResultClear.clear();
    for (auto& anRes : anResult)
    {
        // Skip tasks
        if (anRes.task->status == Task::TaskStatus::CANCELLED)
            continue;
        anResultClear.push_back(anRes);
    }
}

void TaskAnalizer::analizeConnectivity()
{
    bool isConnected;
    for (Task& task : dataBase->tasks)
    {
        if (areSubTasksConnected(task))
            isConnected = true;
        else isConnected = false;
        // To form the resulting vector
        conResult.push_back({ &task, isConnected });
    }
}

bool TaskAnalizer::areSubTasksConnected(const Task& task)
{
    // If connectivity coefficient more than 0.3, then tasks are connected
    return task.connectivity > 0.3;
}

void TaskAnalizer::analizeTask(Task& task, bool isConnected)
{
    // If there are found at least 1 resource
    bool wasFound = false;
    AnalizerResult anRes;
    anRes.task = &task;
    for (Resource& res : dataBase->availableResources)
    {
        // If tasks can be performed, then skip them by iteration
        // in dependencies, whether tasks are connected or aren't called other
        // methods for testing
        if (isConnected && !canBePerformedConnected(*anRes.task, res))
            continue;
        if (!isConnected && !canBePerformedSimple(*anRes.task, res))
            continue;

        wasFound = true;
        // Add pointer on the resource, on which task can be performed
        anRes.resources.push_back(&res);
    }
    // If for tasks exists at least 1 resource, on which it can be performed,
    // then set WAITING status
    if (wasFound)
        anRes.task->status = Task::TaskStatus::WAITING;
    // Otherwise set CANCELLED status
    else
        anRes.task->status = Task::TaskStatus::CANCELLED;
    // Add result to general vector of results
    anResult.push_back(anRes);
}

bool TaskAnalizer::canBePerformedSimple(const Task& task, Resource& res)
{
    // If there aren't enough processors then task cannot be performed
    if (task.resDesc.procCount > res.resDesc.procCount)
        return false;

    // Check compatibility of processor's architecture
    if (task.resDesc.procArch != res.resDesc.procArch)
        return false;

    // Check compatibility of operating systems
    if (task.resDesc.os != res.resDesc.os)
        return false;

    // If processors' speed is not enough, task cannot be performed
    if (task.resDesc.procSpeed > res.resDesc.procSpeed)
        return false;

    // If RAM is not enough, task cannot be performed
    if (task.resDesc.memSize > res.resDesc.memSize)
        return false;

    // If there aren't available space on Hard Drive, task cannot be performed
    if (task.resDesc.discSize > res.resDesc.discSize)
        return false;

    // If all test are passed, task can be performed
    return true;
}

bool TaskAnalizer::canBePerformedConnected(const Task& task, Resource& res)
{
    // If processors not enough, task cannot be performed
    if ((task.count * task.resDesc.procCount) > res.resDesc.procCount)
        return false;

    // If RAM is not enough, task cannot be performed
    if (task.count * task.resDesc.memSize > res.resDesc.memSize)
        return false;

    // If there aren't available space on Hard Drive, task cannot be performed
    if (task.count * task.resDesc.discSize > res.resDesc.discSize)
        return false;

    return canBePerformedSimple(task, res);
}
