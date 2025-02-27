#include "ResourceManager.h"
using RM = ResourceManager;

RM::ResourceRemaining RM::getResourceRemainingData(
    const Resource& res,
    bool considerStatus,
    Task::TaskStatus tStatus
)
{
    RM::ResourceRemaining resRem;
    // �������� ���������� ������� ��������� �������
    // �� ���������� �������, ��� � ��������
    resRem.discSize = res.resDesc.discSize;
    resRem.memSize = res.resDesc.memSize;
    resRem.procCount = res.resDesc.procCount;

    // ������� �� ��������� ������� ���������, 
    // ������ ������ �������
    for (const auto& task : res.performingTasks)
    {
        // ���� ����� ����������� ������ ������� � �� ��
        // �������, ���������� ��������
        if (considerStatus && task->status != tStatus)
            continue;
        // ������� ����� �������� �� ������� ����� ��
        // ��������, �� ����������� � ����� ������
        resRem.discSize -= task->resDesc.discSize * task->count;
        resRem.memSize -= task->resDesc.memSize * task->count;
        resRem.procCount -= task->resDesc.procCount * task->count;
    }

    return resRem;
}

int RM::howManyTasksCanBePerformed(const Task& task, const Resource& res)
{
    int count = 0;
    // �������� ���������� ��� ���� ��������� �������
    RM::ResourceRemaining resRem = RM::getResourceRemainingData(res);
    while (true)
    {
        // ������� �� ��������� ������� ���������, 
        // �� ������ ��� ��������� ���� ������
        resRem.discSize -= task.resDesc.discSize;
        resRem.memSize -= task.resDesc.memSize;
        resRem.procCount -= task.resDesc.procCount;

        // ���� ���� � ���� � ��������� ����� 0, �� ������, ��
        // ����� ���������� ��� �� ��������� ��� ��������� ������,
        // ���� �������� � ����� � ��������� ���������
        if (resRem.discSize < 0 || resRem.memSize < 0 || resRem.procCount < 0)
            break;
        // ���� � ��� ��������� ���������, �������� ����������� �������
        ++count;
    }
    return count;
}

double RM::getMinTimeToFree(const Resource& res)
{
    // ���� ����� � ������ � ��� ����, ��������� 0
    if (res.performingTasks.empty())
        return 0;
    // ������ �������� ���� performTime � ��� �������
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

bool RM::canTaskBeSentToResource(
    const Task& task,
    const Resource& res,
    bool areSubTasksConnected,
    int* outSubTasksCount
)
{
    bool canTaskBeSent = false;
    int count = ResourceManager::howManyTasksCanBePerformed(task, res);

    // ���� ������ ���'����, �� ������ ����� �������� ���,
    // ��� �������� ����� ������� ���������� �� ���� ���
    if (areSubTasksConnected)
    {
        if (count >= task.count)
            canTaskBeSent = true;
    }
    // ������ ��������� ��������� ��������� ����-��� �������
    // ����� �� ������
    else
    {
        if (count > 0)
            canTaskBeSent = true;
    }
    // ���� count ����� �� �������� ������� �����, ��
    // ������������ ��� count �������� ������� �����
    count = (count > task.count) ? task.count : count;

    // �������� �� nullptr
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