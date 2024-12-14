#include "ResourceManager.h"
using RM = ResourceManager;

RM::ResourceRemaining RM::getResourceRemainingData(
    const Resource& res)
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
        // ������� ����� �������� �� ������� ����� ��
        // ��������, �� ����������� � ����� ������
        resRem.discSize -= task.resDesc.discSize * task.count;
        resRem.memSize -= task.resDesc.memSize * task.count;
        resRem.procCount -= task.resDesc.procCount * task.count;
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