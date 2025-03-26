#include "TaskAnalizer.h"

void TaskAnalizer::analizeAllTasks()
{
    // ��������� �����, ��� �������� ���������� ��� ������� ��������
    // ������� ���������� ������ ������� � ���� conResult
    analizeConnectivity();
    for (ConnectivityResult& cr : conResult)
    {
        analizeTask(*cr.task, cr.areConnected);
    }
    // ���������� ����� ������, �� ��������� �������� �� ���������� �������
    clearFromCancelled();
}

void TaskAnalizer::clearFromCancelled()
{
    anResultClear.clear();
    for (auto& anRes : anResult)
    {
        // ���������� ��������
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
        // ������� ������������ ������
        conResult.push_back({ &task, isConnected });
    }
}

bool TaskAnalizer::areSubTasksConnected(const Task& task)
{
    // ���� ���������� ��'������ ����� 0.3, ������ ���������� ��'�������
    return task.connectivity > 0.3;
}

void TaskAnalizer::analizeTask(Task& task, bool isConnected)
{
    bool wasFound = false; // �� ��� ��������� ���� � ���� ������
    AnalizerResult anRes;
    anRes.task = &task;
    for (Resource& res : dataBase->availableResources)
    {
        // ���� �������� �� ���� ���� ��������, ���������� ��������
        // � ��������� �� � ������ ���'���� �� � ������������ ��� ������ �������� 
        if (isConnected && !canBePerformedConnected(*anRes.task, res))
            continue;
        if (!isConnected && !canBePerformedSimple(*anRes.task, res))
            continue;

        wasFound = true;
        // ������ �������� �� ������, �� ����� �������� ���� ���� ��������
        anRes.resources.push_back(&res);
    }
    // ���� ��� �������� ���� ��� ���� ������, �� ����� ���� ���� ����������
    // ������������ ������ WAITING
    if (wasFound)
        anRes.task->status = Task::TaskStatus::WAITING;
    // ������ ������������ ������ CANCELLED
    else
        anRes.task->status = Task::TaskStatus::CANCELLED;
    // ������ ��������� �� ���������� ������� ����������
    anResult.push_back(anRes);
}

bool TaskAnalizer::canBePerformedSimple(const Task& task, Resource& res)
{
    // ���� ��������� �� ���������, �������� ���������� �� ����
    if (task.resDesc.procCount > res.resDesc.procCount)
        return false;

    // ���������� �������� ���������� ���������
    if (task.resDesc.procArch != res.resDesc.procArch)
        return false;

    // ���������� �������� ����������� ������
    if (task.resDesc.os != res.resDesc.os)
        return false;

    // ���� �������� ��������� �� ��������, �������� ���������� �� ����
    if (task.resDesc.procSpeed > res.resDesc.procSpeed)
        return false;

    // ���� ���������� ���'�� �� ���������, �������� ���������� �� ����
    if (task.resDesc.memSize > res.resDesc.memSize)
        return false;

    // ���� ���� �� ����� �� ���������, �������� ���������� �� ����
    if (task.resDesc.discSize > res.resDesc.discSize)
        return false;

    // ���� �� �������� ������� ������, �������� ���� ���� ��������
    return true;
}

bool TaskAnalizer::canBePerformedConnected(const Task& task, Resource& res)
{
    // ���� ��������� �� ���������, �������� ���������� �� ����
    if ((task.count * task.resDesc.procCount) > res.resDesc.procCount)
        return false;
   
    // ���� ���������� ���'�� �� ���������, �������� ���������� �� ����
    if (task.count * task.resDesc.memSize > res.resDesc.memSize)
        return false;

    // ���� ���� �� ����� �� ���������, �������� ���������� �� ����
    if (task.count * task.resDesc.discSize > res.resDesc.discSize)
        return false;

    return canBePerformedSimple(task, res);
}