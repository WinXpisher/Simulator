#include "DistributionMethod.h" 
#include "ResourceManager.h"

using DM = DistributionMethod;
const DM::DMethod DM::dMethods[8] = { LIFO, FCFS, HPF, BACKFILL, SIMPLEX, SMART, MFQS, Penguin, };

bool LIFO::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    if (tasks.empty()) // ����������, �� � ��������
        return false;

    // �������� ������ ��������
    outAnResult = tasks.back();

    // ��������� ���� � ������
    tasks.pop_back();
    return true;
}

bool FCFS::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    // ���� ������� ����, ��������� false 
    if (tasks.empty())
        return false;
    // �������� ����� �������� � ���� 
    TaskAnalizer::AnalizerResult& taskResult = tasks.front();
    // �������� ������� �������� � ���������� 
    outAnResult = taskResult;
    // ��������� ��������� �������� � ������� 
    tasks.erase(tasks.begin());
    return true;
}

bool HPF::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    if (tasks.empty())
        return false;

    // ��������� �������� � �������� ����������
    auto highestPriorityTaskIter = max_element(
        tasks.begin(), tasks.end(),
        [](const TaskAnalizer::AnalizerResult& a, const TaskAnalizer::AnalizerResult& b) {
            return a.task->priority < b.task->priority;
        });

    // ������������� �������� � ����������, ��� �������� ���������
    outAnResult = *highestPriorityTaskIter;
    tasks.erase(highestPriorityTaskIter);
    return true;
}

//bool BACKFILL::nextTask(
//    vector<TaskAnalizer::AnalizerResult>& tasks,
//    TaskAnalizer::AnalizerResult& outAnResult,
//    Resource*& outRes) const
//{
//    if (tasks.empty())
//        return false;
//
//    // �������� �� ������� ��� ����������� 򳺿, ��� ���� ���� �������� � ���������� ���������
//    for (auto it = tasks.begin(); it != tasks.end(); ++it)
//    {
//        for (auto& res : it->resources)
//        {
//            // ����������, �� ������ ������� ������� ��������
//            if (res->resDesc.procCount >= it->task->resDesc.procCount &&
//                res->resDesc.memSize >= it->task->resDesc.memSize &&
//                res->resDesc.discSize >= it->task->resDesc.discSize)
//            {
//                // ������� ������� ��������
//                outAnResult = *it;
//                outRes = res;
//                tasks.erase(it);
//                return true;
//            }
//        }
//    }
//
//    // ���� �� �������� ���������� ��������
//    return false;
//}



double calculateScore(const Task* task, const Resource* resource)
{
    static const double PRIORITY_WEIGHT = 10.0;
    static const double PERFORM_TIME_WEIGHT = -1.0;
    static const double CONNECTIVITY_WEIGHT = 5.0;
    static const double BANDWIDTH_WEIGHT = 2.0;

    return task->priority * PRIORITY_WEIGHT +
        task->performTime * PERFORM_TIME_WEIGHT +
        task->connectivity * CONNECTIVITY_WEIGHT +
        resource->bandwidth * BANDWIDTH_WEIGHT;
}

Resource* findOptimizedResource(
    const Task& task,
    const vector<Resource*>& freeResources,
    bool areSubTasksConnected)
{
    if (freeResources.empty())
        return nullptr;

    Resource* bestResource = nullptr;
    double bestScore = std::numeric_limits<double>::lowest();
    for (auto& resource : freeResources)
    {
        // �������� �������� ���������� �������
        auto remaining = ResourceManager::getResourceRemainingData(*resource);

        // ����������� ������ �������
        double score = 0.0;
        score += remaining.procCount * 60; // ������ ��������� - �����
        score += remaining.memSize * 3;   // ������ ���'�� - �����
        score += remaining.discSize;  // ������ ��������� �������� - �����
        score += resource->bandwidth * 4; // ��������� ��������
        
        score -= resource->resDesc.procSpeed * 20;
        //// ���� �������� ���'�����, ��������� ������� ���������
        //if (areSubTasksConnected)
        //    score -= abs(resource->resDesc.procCount - task.resDesc.procCount);

        // ��������� ��������� ������, ���� ������ ����
        if (score > bestScore)
        {
            bestScore = score;
            bestResource = resource;
        }
    }

    return bestResource;
}

Resource* BACKFILL::nextResource(
    TaskAnalizer::AnalizerResult& anResult
) const
{
    //return nullptr;
    DataBase db; // ����������� ���� �����
    TaskAnalizer analyzer(&db);
    vector<Resource*> freeResources;
    for (auto& res : anResult.resources)
    {
        // ����������, �� ���� �������� ���������� �� ������ � �������� ������
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

bool BACKFILL::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    if (tasks.empty())
        return false;

    DataBase db; // ����������� ���� �����
    TaskAnalizer analyzer(&db);
    Resource* resToSelect = nullptr;
    // �������� �� ������� ��� ����������� 򳺿, ��� ���� ���� �������� � ���������� ���������
    for (auto it = tasks.begin(); it != tasks.end(); ++it)
    {
        // ����������, �� ���� �������� ���������� �� ������ � �������� ������
        if (resToSelect = ResourceManager::findAnyFreeResource(
            *(*it).task,
            (*it).resources,
            analyzer.areSubTasksConnected(*(*it).task)))
        {
            // ������� ������� ��������
            outAnResult = *it;
            tasks.erase(it);
            return true;
        }
    }
    // �������� ������ ��������
    outAnResult = tasks.back();

    // ��������� ���� � ������
    tasks.pop_back();
    return true;
}

bool SIMPLEX::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    // ���� ���� ������� �������� false
    if (tasks.empty())
        return false;

    // ����� �������� �� ������ �������,
    size_t selectedIdx = 0;
    for (size_t i = 1; i < tasks.size(); ++i)
    {
        // ���� �������� �� ������ �������
        if (tasks[i].task->count > tasks[selectedIdx].task->count ||
            tasks[i].task->connectivity > tasks[selectedIdx].task->connectivity ||
            tasks[i].task->subTaskSize > tasks[selectedIdx].task->subTaskSize)
        {
            selectedIdx = i;
        }
    }

    // ����������� ���������� ��������
    outAnResult = tasks[selectedIdx];

    // �������� ������� �������� � ������
    tasks.erase(tasks.begin() + selectedIdx);

    return true;
}

bool SMART::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    if (tasks.empty())
        return false;

    TaskAnalizer::AnalizerResult* selectedTask = &tasks[0];
    Resource* selectedResource = tasks[0].resources[0];
    double bestScore = std::numeric_limits<double>::min();;

    // ��������� �� ��� ��������� � �������� ��� ������ ������������ �������
    for (auto& taskResult : tasks)
    {
        for (auto& resource : taskResult.resources)
        {
            // ���������� ������ �������� �� ��������� �������� �������
            double score = 0.0;
            score += taskResult.task->priority * 10;  // �������� ��������
            score -= taskResult.task->performTime;    // ��� ��������� �������� (������ ��� - �����)
            score += taskResult.task->connectivity * 5;  // ���������� ��'������
            score += resource->bandwidth * 2;           // ��������� �������� �������

            // ���� �������� �� ����� ������, �������� ����
            if (score > bestScore)
            {
                bestScore = score;
                selectedTask = &taskResult;
                selectedResource = resource;
            }
        }
    }

    // ���������� �������� �������� � ������
    outAnResult = *selectedTask;
    //outRes = selectedResource;

    // ��������� ������ �������� � ������
    tasks.erase(remove_if(tasks.begin(), tasks.end(),
        [&](const TaskAnalizer::AnalizerResult& task) {
            return task.task->id == selectedTask->task->id;
        }),
        tasks.end());
    return true;
}



//bool MFQS::nextTask(
//    vector<TaskAnalizer::AnalizerResult>& tasks,
//    TaskAnalizer::AnalizerResult& outAnResult,
//    Resource*& outRes) const
//{
//    // ������ �� �������� �� ������� �����, ���� ���� �� �� ���������
//    for (auto& taskResult : tasks)
//    {
//        addToQueue(taskResult, 0); // ��������� �������� �������� � ����� �����
//    }
//
//    // ��������� �� ������ (� ��������� �� ���������� ����)
//    for (size_t i = 0; i < priorityQueues.size(); ++i)
//    {
//        auto& queue = priorityQueues[i];
//        while (!queue.empty())
//        {
//            auto task = queue.front(); // ������ ����� �������� � �����
//            queue.pop_front();         // ��������� ���� � �����
//
//            // ����������, �� �������� ������ ��� ��������� ��������
//            for (auto& resource : task.resources)
//            {
//                if (resource->resDesc.procCount >= task.task->resDesc.procCount &&
//                    resource->resDesc.memSize >= task.task->resDesc.memSize &&
//                    resource->resDesc.discSize >= task.task->resDesc.discSize)
//                {
//                    // ���������� �������� � ������
//                    outAnResult = task;
//                    outRes = resource;
//                    return true;
//                }
//            }
//
//            // ���� ������ �� ��������, �������� ���������� � ����� �����
//            if (i + 1 < priorityQueues.size())
//            {
//                addToQueue(task, i + 1);
//            }
//        }
//    }
//
//    // �������� ������ ��������
//    outAnResult = tasks.back();
//
//    // ��������� ���� � ������
//    tasks.pop_back();
//}

//bool MFQS::nextTask(
//    vector<TaskAnalizer::AnalizerResult>& tasks,
//    TaskAnalizer::AnalizerResult& outAnResult,
//    Resource*& outRes) const
//{
//    if (tasks.empty()) // ���� ������� ����
//        return false;
//
//    DataBase db; // ����������� ���� �����
//    TaskAnalizer analyzer(&db); // �������� ��������� ��'���� `TaskAnalizer`
//
//    // ��������� �� ��� ����� ����
//    for (size_t i = 0; i < priorityQueues.size(); ++i)
//    {
//        auto& queue = priorityQueues[i];
//
//        while (!queue.empty())
//        {
//            auto task = queue.front();
//            queue.pop_front();
//
//            bool areConnected = analyzer.areSubTasksConnected(*task.task);
//
//            Resource* suitableResource = ResourceManager::findAnyFreeResource(
//                *task.task, task.resources, areConnected
//            );
//
//            if (suitableResource)
//            {
//                outAnResult = task;
//                outRes = suitableResource;
//                return true;
//            }
//
//            if (i + 1 < priorityQueues.size())
//                addToQueue(task, i + 1);
//        }
//    }
//
//    // �������� ������ ��������
//    outAnResult = tasks.back();
//
//    // ��������� ���� � ������
//    tasks.pop_back();
//}

//bool Penguin::nextTask(
//    vector<TaskAnalizer::AnalizerResult>& tasks,
//    TaskAnalizer::AnalizerResult& outAnResult,
//    Resource*& outRes) const
//{
//    if (tasks.empty()) // ����������, �� � ��������
//        return false;
//
//    TaskAnalizer::AnalizerResult* selectedTask = nullptr;
//    Resource* selectedResource = nullptr;
//    double minWeight = std::numeric_limits<double>::max(); // ��� ������ ���������� ��������
//
//    // ��������� �� ��� ��������� � ��������
//    for (auto& taskResult : tasks)
//    {
//        for (auto& resource : taskResult.resources)
//        {
//            // ���������� "����" ������� ��������
//            double weight = taskResult.task->resDesc.procCount +
//                taskResult.task->resDesc.memSize +
//                taskResult.task->resDesc.discSize;
//
//            // ���� �������� �� ����� ����, �� �������� ��������, �������� ����
//            if (weight < minWeight)
//            {
//                minWeight = weight;
//                selectedTask = &taskResult;
//                selectedResource = resource;
//            }
//        }
//    }
//
//    // ���� �� �������� ���������� ��������
//    if (selectedTask == nullptr)
//        return false;
//
//    // ���������� �������� �������� � ������
//    outAnResult = *selectedTask;
//    //outRes = selectedResource;
//
//    // ��������� ������ �������� � ������
//    tasks.erase(remove_if(tasks.begin(), tasks.end(),
//        [&](const TaskAnalizer::AnalizerResult& task) {
//            return task.task->id == selectedTask->task->id;
//        }),
//        tasks.end());
//    return true;
//}


Resource* Penguin::nextResource(
    TaskAnalizer::AnalizerResult& anResult
) const
{
    DataBase db; // ����������� ���� �����
    TaskAnalizer analyzer(&db);
    vector<Resource*> freeResources;
    for (auto& res : anResult.resources)
    {
        // ����������, �� ���� �������� ���������� �� ������ � �������� ������
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
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    if (tasks.empty()) // ����������, �� � ��������
        return false;

    TaskAnalizer::AnalizerResult* selectedTask = nullptr;
    Resource* selectedResource = nullptr;
    double minWeight = std::numeric_limits<double>::max(); // ��� ������ ���������� ��������

    // ��������� �� ��� ��������� � ��������
    for (auto& taskResult : tasks)
    {
        for (auto& resource : taskResult.resources)
        {
            // ���������� "����" ������� ��������
            double weight = 6 * taskResult.task->count *
                taskResult.task->resDesc.procCount +
                8 * taskResult.task->resDesc.memSize +
                taskResult.task->resDesc.discSize;

            // ���� �������� �� ����� ����, �� �������� ��������, �������� ����
            if (weight < minWeight)
            {
                minWeight = weight;
                selectedTask = &taskResult;
                selectedResource = resource;
            }
        }
    }

    // ���� �� �������� ���������� ��������
    if (selectedTask == nullptr)
        return false;

    // ���������� �������� �������� � ������
    outAnResult = *selectedTask;
    //outRes = selectedResource;

    // ��������� ������ �������� � ������
    tasks.erase(remove_if(tasks.begin(), tasks.end(),
        [&](const TaskAnalizer::AnalizerResult& task) {
            return task.task->id == selectedTask->task->id;
        }),
        tasks.end());
    return true;
}