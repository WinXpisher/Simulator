#include "DistributionMethod.h" 
#include "ResourceManager.h"


using DM = DistributionMethod;
const DM::DMethod DM::dMethods[8] = { LIFO, FCFS, HPF, BACKFILL, SIMPLEX, SMART, MFQS, Penguin, };

bool LIFO::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const
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
    TaskAnalizer::AnalizerResult& outAnResult
) const
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
    TaskAnalizer::AnalizerResult& outAnResult
) const
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


bool BACKFILL::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const
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
    TaskAnalizer::AnalizerResult& outAnResult
) const
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
    TaskAnalizer::AnalizerResult& outAnResult
) const
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

    // ��������� ������ �������� � ������
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
    if (tasks.empty()) return false; // ����������, �� � ��������

    // ����������, �� �������� ��������� �� ������ ���� (������� � 3 ������)
    vector<vector<TaskAnalizer::AnalizerResult>> queues(3); // 3 ���� ����

    // ������� ������� �� ������ �� �� ����������
    for (auto& task : tasks) {
        if (task.task->priority >= 10) {
            queues[0].push_back(task); // ������� ��������
        }
        else if (task.task->priority >= 5) {
            queues[1].push_back(task); // ������� ��������
        }
        else {
            queues[2].push_back(task); // ������� ��������
        }
    }

    // ��������� �� ������ �� ��������� ��������� �� ����������
    for (auto& queue : queues) {
        if (!queue.empty()) {
            // �������� �������� � �����
            outAnResult = queue.front();
            queue.erase(queue.begin()); // ��������� ���� � �����

            // ��������� ����� �������� � ����������� �������
            tasks.erase(remove_if(tasks.begin(), tasks.end(), [&](const TaskAnalizer::AnalizerResult& task) {
                return task.task->id == outAnResult.task->id;
                }), tasks.end());
            return true;
        }
    }

    return false; // ���� ������� � ������ ����
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
        // ��������� ��������� ������, ���� ������ ����
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
    TaskAnalizer::AnalizerResult& outAnResult
) const
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

    // ��������� ������ �������� � ������
    tasks.erase(remove_if(tasks.begin(), tasks.end(),
        [&](const TaskAnalizer::AnalizerResult& task) {
            return task.task->id == selectedTask->task->id;
        }),
        tasks.end());
    return true;
}