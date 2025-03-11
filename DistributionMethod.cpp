#include "DistributionMethod.h" 

using DM = DistributionMethod;
const DM::DMethod DM::dMethods[6] = { LIFO, FCFS, HPF, BACKFILL, SIMPLEX, RR };

bool LIFO::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
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

bool HPF::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    // ���� ������� ����, ��������� false 
    if (tasks.empty())
        return false;

    // ��������� �������� � �������� ���������  
    auto highestPriorityTaskIter = max_element(tasks.begin(), tasks.end(),
        [](const TaskAnalizer::AnalizerResult& a, const TaskAnalizer::AnalizerResult& b) {
            return a.task->priority < b.task->priority;
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
    Resource*& outRes) const
{
    // ���� ���� ������� �������� false 
    if (tasks.empty())
        return false;

    // ����� �������� �� ������ �������, ���������, ���� ���������� ���� ��������� 
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

bool RR::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    // ���� ���� �������, ��������� false
    if (tasks.empty()) {
        return false; 
    }

    // ������� �������� �� ����������� ��������
    outAnResult = tasks[currentIndex];
    // ���� �������� �� ���������, ��������� ���� � ����� ����� (���� �� 
    // ��������� �� ����� ����). � ��������� ����������� ������ ���������,
    // �� �������� ���������, �� ���� �� �������������� ���� ����� ������ ����
    tasks.push_back(tasks[currentIndex]);

    // ��������� ��������� �������� � ������ �������
    tasks.erase(tasks.begin() + currentIndex);

    // ��������� ������ ������
    currentIndex %= tasks.size();

    return true;
}