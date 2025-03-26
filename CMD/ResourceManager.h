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
    // ����� ������� ������� ���������� �������, �� �� ����� �������.
    // �������� considerStatus ��������������� � ��������� �����������
    // � ���� ������������ �� true, �� ������������ ����� � ��������,
    // � ���� ������ ������� � ���������� tStatus.
    static ResourceRemaining getResourceRemainingData(
        const Resource& res,
        bool considerStatus=false,
        Task::TaskStatus tStatus=Task::TaskStatus::RUNNING
    );
    // ������ ����� ������� �������� ���� ���� �������� �� ������ ������
    static int howManyTasksCanBePerformed(const Task& task, const Resource& res);
    // �� ����� ��������� �� �������� �� ������� �������� ��� �������.
    // ���� �������� �������� outSubTasksCount � ����� ������� true, ��
    // � �� ������ ���� �������� ������� �����, �� ������ ���� ������� ��������.
    static bool canTaskBeSentToResource(
        const Task& task,
        const Resource& res,
        bool areSubTasksConnected,
        int* outSubTasksCount = nullptr
    );
    // �������� ��������� ��� ��� ��������� ������� ���� � �� ���� ������
    static double getMinTimeToFree(const Resource& res);
    // ������ ����-���� ������, ���� ���� �������� ���� ��������
    // �������� ��� ������� (�������� �� ��'������ �������).
    // ���� ������ ����, ����� ������� nullptr.
    static Resource* findAnyFreeResource(
        const Task& task,
        const vector<Resource*>& resources,
        bool areSubTasksConnected
    );
    // ����������� ������ ������� � ��������
    static double calcResourceStagnation(vector<Resource>& resources);
    // ������ ������, �� ����� ��� ���������� ��������
    static const Resource* findResourceTaskIsPerfOn(
        const Task* task,
        const vector<Resource>& resources
    );
};