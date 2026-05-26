#include "DistributionMethod.h" 
#include "ResourceManager.h"
#include <algorithm>
#include <limits>

using DM = DistributionMethod;
const DM::DMethod DM::dMethods[8] = { LIFO, FCFS, HPF, BACKFILL, SIMPLEX, SMART, MFQS, Penguin, };

bool LIFO::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const
{
    if (tasks.empty()) // Check, if task exists
        return false;

    // Get last task
    outAnResult = tasks.back();

    // Pop the task from list
    tasks.pop_back();
    return true;
}

bool FCFS::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const
{
    // If there no tasks, then return false
    if (tasks.empty())
        return false;
    // Get first task in queue
    TaskAnalizer::AnalizerResult& taskResult = tasks.front();
    // Record output's tasks from result
    outAnResult = taskResult;
    // Delete the processed task from vector
    tasks.erase(tasks.begin());
    return true;
}

bool HPF::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const
{
    if (tasks.empty())
        return false;

    // Find task with the highest priority
    auto highestPriorityTaskIter = max_element(
        tasks.begin(), tasks.end(),
        [](const TaskAnalizer::AnalizerResult& a, const TaskAnalizer::AnalizerResult& b) {
            return a.task->priority < b.task->priority;
        });

    // Directly working with the iterator to avoid copying
    outAnResult = *highestPriorityTaskIter;
    tasks.erase(highestPriorityTaskIter);
    return true;
}


bool BACKFILL::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
    ) const
{
    if (tasks.empty())
        return false;

    DataBase db; // Database initialization
    TaskAnalizer analyzer(&db);

    // Iterate through tasks to find a task that can be executed
    // with currently available resources
    for (auto it = tasks.begin(); it != tasks.end(); ++it)
    {
        // Check if the task can be performed on any free resource at the moment
        if (ResourceManager::findAnyFreeResource(
                *(*it).task,
                (*it).resources,
                analyzer.areSubTasksConnected(*(*it).task)))
        {
            // If task found select it, remove from the queue, and return
            outAnResult = *it;
            tasks.erase(it);
            return true;
        }
    }

    // Fallback logic if no task matches the available resources:
    // Selects the oldest task in the queue to prevent starvation
    outAnResult = tasks.front();
    tasks.erase(tasks.begin());

    return true;
}

bool SIMPLEX::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const
{
    // If there are no tasks, then return false
    if (tasks.empty())
        return false;

    // Find the task followed by a certain rule
    size_t selectedIdx = 0;
    for (size_t i = 1; i < tasks.size(); ++i)
    {
        // Slect the task followed by a certain rule
        if (tasks[i].task->count > tasks[selectedIdx].task->count ||
            tasks[i].task->connectivity > tasks[selectedIdx].task->connectivity ||
            tasks[i].task->subTaskSize > tasks[selectedIdx].task->subTaskSize)
        {
            selectedIdx = i;
        }
    }

    // Put found task into output result variable
    outAnResult = tasks[selectedIdx];

    // Delete selected task from list
    tasks.erase(tasks.begin() + selectedIdx);

    return true;
}


bool SMART::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const
{
    // If there are no tasks, then return false
    if (tasks.empty())
        return false;

    TaskAnalizer::AnalizerResult* selectedTask = &tasks[0];
    Resource* selectedResource = tasks[0].resources[0];
    double bestScore = std::numeric_limits<double>::min();;

    // Iterate through all tasks and resources for finding a optimal variant
    for (auto& taskResult : tasks)
    {
        for (auto& resource : taskResult.resources)
        {
            // Calculate task's score followed by a few rules
            double score = 0.0;
            score += taskResult.task->priority * 10;  // task priority
            score -= taskResult.task->performTime;    // performing task (small amount of time = better performing)
            score += taskResult.task->connectivity * 5;  // connectivity coefficient
            score += resource->bandwidth * 2;           // resource's bandwidth

            // If task has best score, then select the task
            if (score > bestScore)
            {
                bestScore = score;
                selectedTask = &taskResult;
                selectedResource = resource;
            }
        }
    }

    // Put found task and resource into output result variable
    outAnResult = *selectedTask;

    // Delete selected task from list
    tasks.erase(remove_if(tasks.begin(), tasks.end(),
        [&](const TaskAnalizer::AnalizerResult& task) {
            return task.task->id == selectedTask->task->id;
        }),
        tasks.end());
    return true;
}


bool MFQS::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const {
    // If there are no tasks, then return false
    if (tasks.empty()) return false;

    // Let's assume, taks are distributed by queue's level (example with 3 levels)
    vector<vector<TaskAnalizer::AnalizerResult>> queues(3); // 3 queue's levels

    // Task's distribution by queue with their priority
    for (auto& task : tasks) {
        if (task.task->priority >= 10) {
            queues[0].push_back(task); // high priority
        }
        else if (task.task->priority >= 5) {
            queues[1].push_back(task); // medium priority
        }
        else {
            queues[2].push_back(task); // low priority
        }
    }

    // Iterate throgh queues from the highest to the lowest
    for (auto& queue : queues) {
        if (!queue.empty()) {
            // Get tasks from queue
            outAnResult = queue.front();
            queue.erase(queue.begin()); // delete the task from queue

            // Also delete the task from beginning vector
            tasks.erase(remove_if(tasks.begin(), tasks.end(), [&](const TaskAnalizer::AnalizerResult& task) {
                return task.task->id == outAnResult.task->id;
                }), tasks.end());
            return true;
        }
    }

    // If there are no tasks
    return false;
}


Resource* findOptimizedResource(
    const Task& task,
    const vector<Resource*>& freeResources,
    bool areSubTasksConnected)
{
    // If there are no tasks, then return false
    if (freeResources.empty())
        return nullptr;

    Resource* bestResource = nullptr;
    double bestScore = std::numeric_limits<double>::lowest();
    for (auto& resource : freeResources)
    {
        // Get remaning components of resource
        auto remaining = ResourceManager::getResourceRemainingData(*resource);

        // Calculate resource's score
        double score = 0.0;
        score += remaining.procCount * 60;  // more processors = better result
        score += remaining.memSize * 3;     // more RAM = better result
        score += remaining.discSize;        // more HardDrive size = better result
        score += resource->bandwidth * 4;   // resource's bandwidth

        // Update the best resource, if it has best socre
        if (score > bestScore)
        {
            bestScore = score;
            bestResource = resource;
        }
    }

    return bestResource;
}


Resource* Penguin::nextResource(
    TaskAnalizer::AnalizerResult& anResult
) const
{
    // Initialization a database
    DataBase db;
    TaskAnalizer analyzer(&db);
    vector<Resource*> freeResources;
    for (auto& res : anResult.resources)
    {
        // Check, whether task can be performed on the resource at the moment
        if (ResourceManager::canTaskBeSentToResource(
            *anResult.task,
            *res,
            analyzer.areSubTasksConnected(*anResult.task)))
        {
            freeResources.push_back(res);
        }
    }

    return findOptimizedResource(
        *anResult.task,
        freeResources,
        analyzer.areSubTasksConnected(*anResult.task)
    );
}

bool Penguin::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const
{
    // If there are no tasks, then return false
    if (tasks.empty())
        return false;

    TaskAnalizer::AnalizerResult* selectedTask = nullptr;
    Resource* selectedResource = nullptr;
    // Searching for the minimum value
    double minWeight = std::numeric_limits<double>::max();

    // Iterate through all tasks and resources
    for (auto& taskResult : tasks)
    {
        for (auto& resource : taskResult.resources)
        {
            // Calculate "weights" for tasks' resources
            double weight = 6 * taskResult.task->count *
                taskResult.task->resDesc.procCount +
                8 * taskResult.task->resDesc.memSize +
                taskResult.task->resDesc.discSize;

            // If the task has a small "weight", than previous has the best,
            // then select its
            if (weight < minWeight)
            {
                minWeight = weight;
                selectedTask = &taskResult;
                selectedResource = resource;
            }
        }
    }

    // If it hasn't found a needed task
    if (selectedTask == nullptr)
        return false;

    // Put the found task and resource into autput result variable
    outAnResult = *selectedTask;

    // Delete selected task from list
    tasks.erase(remove_if(tasks.begin(), tasks.end(),
        [&](const TaskAnalizer::AnalizerResult& task) {
            return task.task->id == selectedTask->task->id;
        }),
        tasks.end());
    return true;
}
