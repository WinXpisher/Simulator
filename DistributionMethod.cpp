#include "DistributionMethod.h" 

using DM = DistributionMethod;
const DM::DMethod DM::dMethods[5] = { LIFO, FCFS, HPF, BACKFILL, SIMPLEX };

bool LIFO::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource* outRes) const
{
    // ���� ������� �� ����������, ��������� false 
    if (tasks.empty())
        return false;

    // �������� ������ �������� � ������� ����� 
    // � ��������� ���� � ������� 
    outAnResult = tasks.back();
    tasks.pop_back();
    return true;
}

bool FCFS::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource* outRes) const
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

bool HPF::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource* outRes) const
{
    // ���� ������� ����, ��������� false 
    if (tasks.empty())
        return false;

    // ��������� �������� � �������� ���������  
    auto highestPriorityTaskIter = max_element(tasks.begin(), tasks.end(),
        [](const TaskAnalizer::AnalizerResult& a, const TaskAnalizer::AnalizerResult& b) {
            return a.task.priority < b.task.priority;
        });
    // ��������� �������� � �������� ��������� 
    TaskAnalizer::AnalizerResult highestPriorityTask = *highestPriorityTaskIter;
    outAnResult = highestPriorityTask;
    // ��������� ��������� �������� � ������� 
    tasks.erase(highestPriorityTaskIter);
    return true;
}

//bool BACKFILL::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
//    TaskAnalizer::AnalizerResult& outAnResult,
//    Resource* outRes) const
//{
//    // ���� ���� ������� ��������� false 
//    if (tasks.empty())
//        return false;
//
//    TaskAnalizer::AnalizerResult selectedTask;
//    bool found = false;
//
//    for (auto it = tasks.begin(); it != tasks.end(); ++it)
//    {
//        // ��� ����� ������ � ���������, ��� ������������� ��� �������� � ����������� ������� �������, �� � 
//    }
//
//    // ���� ���� �������� �������� �� ������� 
//    if (!found)
//    {
//        return false;
//    }
//    outAnResult = selectedTask;
//    // ��������� ������ �������� � ������ ������� 
//    tasks.erase(remove(tasks.begin(), tasks.end(), selectedTask), tasks.end());
//    return true;
//}

bool SIMPLEX::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource* outRes) const
{
    // ���� ���� ������� �������� false 
    if (tasks.empty())
        return false;

    // ����� �������� �� ������ �������, ���������, ���� ���������� ���� ��������� 
    size_t selectedIdx = 0;
    for (size_t i = 1; i < tasks.size(); ++i)
    {
        // ���� �������� �� ������ ������� (���������, �� ���������� ���� ��������� ��� ���� ������ ������ �������) 
        if (tasks[i].task.performTime < tasks[selectedIdx].task.performTime)
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