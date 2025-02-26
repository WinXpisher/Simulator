#pragma once
#include <string>
#include <vector>
#include <list>

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
private:
    // � ������� ��������� ���
    static std::string globalId;
public:
    // ��������� ���������� ��� � ���������� ������
    // A, B, C, ..., Z, AA, AB, ..., ZZ, AAA, ...
    static std::string generateId();
    
    std::string id;
    ResourceDescriptor resDesc; // ���� �������
    double bandwidth; // ��������� �������� ������ (�� ������� �� �������)
    // ������, �� ����������� � ����� ������ ����
    // ���� ������ ������, �� ����� ����� ����
    vector<Task*> performingTasks;
};

// ���������� �������, ���'����� � ����������
struct SimulationInfo
{
    // �������� �� ���������� �������� (��� �� ������� �������
    // �������������� � nullptr)
    Task* parentTaskPtr;
    double waitingTime; // ������ ���� �������� ����
    double timePerformed; // ������ ���� �������� ��� ����������
    // ������ �������� (���� ������ �� ������� ���������� ��'������,
    // �������� ����� �������� �� �������, ��� ���������� �� �������
    // �������). ������������� list, ��� �����������, �� ���'��� �� ����
    // ���������������� 
    std::list<Task> childTasks;
};

// ��������, ��� ������ ����� �����
struct Task
{
private:
    // � ������� �������� ���
    static int globalId;
public:
    // ����� �������� ������ ���������� ������������� �� 
    // ������� �����. ������������ ������ �� ������� �����
    // parentTask � �������� ������� ����� ������� �������.
    static int getRemainingSubTasksCount(const Task& parentTask);
    // ��������� ���������� ���
    static std::string generateId();

    enum TaskStatus
    {
        WAITING,   // �������� �����
        CANCELLED, // �������� ���������
        SENDING,   // �������� ����������� �� ������
        RUNNING,   // �������� ��������
        PERFORMED, // �������� ��������
        // �������� �������� �� ����� �������� (������ ��������),
        // ��� ���� ������� ��� �� �����
        DIVIDED,
        // �������� �������� �� ����� �������� (������ ��������)
        // � �� �� ������ �������� ��� ������������ ��� �����������
        // (��� ������ �������), ��� �� ��������
        DIVIDED_RUNNING
        /* ���� �������� �� ������ DIVIDED_RUNNING � �� ����
            ������ �������� ����� ������ PERFORMED, �� ���� ��������
            ��������� ��������� � ���� ������������ ������ PERFORMED.
        */
    };
    std::string id;
    TaskStatus status; // ������ ��������
    int count; // ������� ����� � �������
    ResourceDescriptor resDesc; // ���� �������, �� ����� ����� ������ ���� ����������
    double connectivity; // ���������� ��'������ ����� � �������
    int priority; // �������� ��������
    double performTime; // ��� ��������� ���� ������
    double subTaskSize; // ����� ������ (����������� �����, ���� ���� �����������)
    SimulationInfo simulationInfo; // ��������� ����������, ���'����� � ����������
};

struct DataBase
{
    vector<Task> tasks; // ��������, �� ����� ��������
    vector<Resource> availableResources; // ������� �������
};