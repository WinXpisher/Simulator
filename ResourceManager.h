#pragma once
#include "SimulatorPrimitives.h"

class ResourceManager
{
public:
    // ������, �� ����������� � ����� ������ �������� �����
    // ������� ���������, ���'�� � ��
    // ��������� �������� ���� ���������� �������
    struct ResourceRemaining
    {
        int procCount; // ������� ���������
        int memSize; // ��'�� ���������� ���'��
        int discSize; // ��������� ����� ���������
    };
    // ����� ������� ������� ���������� �������, �� �� ����� �������
    static ResourceRemaining getResourceRemainingData(const Resource& res);
    // ������ ����� ������� �������� ���� ���� �������� �� ������ ������
    static int howManyTasksCanBePerformed(const Task& task, const Resource& res);
};