#pragma once
#include <string>
#include <vector>
#include <list>

#include <fstream>

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

    friend std::istream& operator>>(
        std::istream& is,
        ResourceDescriptor& resDesc
    );
    friend std::ostream& operator<<(
        std::ostream& os,
        const ResourceDescriptor& resDesc
    );
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

    friend std::istream& operator>>(
        std::istream& is,
        Resource& res
        );
    friend std::ostream& operator<<(
        std::ostream& os,
        const Resource& res
        );
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

    friend std::istream& operator>>(
        std::istream& is,
        Task& task
        );
    friend std::ostream& operator<<(
        std::ostream& os,
        const Task& task
        );
};

struct DataBase
{
    vector<Task> tasks; // ��������, �� ����� ��������
    vector<Resource> availableResources; // ������� �������
};
