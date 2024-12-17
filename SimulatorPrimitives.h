#pragma once
#include <string>
#include <vector>

using namespace std;
// �������� �������
struct ResourceDescriptor
{
    string procArch; // ����������� ���������
    string os; // ���������� �������
    int procCount; // ������� ���������
    int procSpeed; // �������� ���������
    int memSize; // ��'�� ���������� ���'��
    int discSize; // ��������� ����� ���������
};

struct Task;
struct Resource
{
    ResourceDescriptor resDesc; // ���� �������
    double bandwidth; // ��������� �������� ������ (�� ������� �� �������)
    // ������, �� ����������� � ����� ������ ����
    // ���� ������ ������, �� ����� ����� ����
    vector<Task> performingTasks;
};

// ��������, ��� ������ ����� �����
struct Task
{
    enum TaskStatus
    {
        WAITING,
        CANCELLED
    };
    TaskStatus status; // ������ ��������
    int count; // ������� ����� � �������
    ResourceDescriptor resDesc; // ���� �������, �� ����� ����� ������ ���� ����������
    double connectivity; // ���������� ��'������ ����� � �������
    int priority; // �������� ��������
    double performTime; // ��� ��������� ������
    double subTaskSize; // ����� ������ (����������� �����, ���� ���� �����������)
};

struct DataBase
{
    vector<Task> tasks; // ��������, �� ����� ��������
    vector<Resource> availableResources; // ������� �������
};