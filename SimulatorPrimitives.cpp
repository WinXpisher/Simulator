#include "SimulatorPrimitives.h"

int Task::globalId = 1;
std::string Resource::globalId = "";

std::string Resource::generateId()
{
    if (globalId.empty()) 
    {
        globalId = "A";
        return globalId;
    }

    int i = globalId.size() - 1;
    // ���� ����� � ��������� � ������ - Z,
    // �������� �� �� ����� - A
    while (i >= 0 && globalId[i] == 'Z') 
    {
        globalId[i] = 'A';
        --i;
    }
    // ���� �� ����� ���� Z, ������ ����� ������
    if (i < 0)
        globalId.insert(globalId.begin(), 'A');
    // ������, ������ �������� ������� (� ����) �����,
    // ��� �� ���� Z �� �������� �� �������
    else
        globalId[i]++;

    return globalId;
}

std::string Task::generateId()
{
    return std::to_string(globalId++); 
}

int Task::getRemainingSubTasksCount(const Task& parentTask)
{
    int countToSubstract = 0;
    // ���������� ���� ����� ��� ������� ������� ��������� ��������
    for (const Task& child : parentTask.simulationInfo.childTasks)
    {
        countToSubstract += child.count;
    }
    // ��������� ������ ����� ������������ �������� � ���������� ����
    return parentTask.count - countToSubstract;
}