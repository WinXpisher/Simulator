#pragma once
#include <vector>
#include "SimulatorPrimitives.h"
#include "DistributionMethod.h"
#include "TaskAnalizer.h"
using namespace std;

class SimulationEnvironment
{
private:
    using DM = DistributionMethod;
    struct SimulationResult;

    TaskAnalizer taskAnalizer; // ��'���, ���� ���� ���������� ����� ��������, ����� ����������
    vector<DM*> distributionMethods; // ������ ��������
    DataBase* dataBase; // ���� ����� � ���������� ��������� �� ����������
    vector<SimulationResult> simResults; // ���� ���������� ��� ������� ������ ��������

    // ����� �������� ������ ������ ��������
    void initDMethods();
    // ����� ������� ��������� ��� ������ ����������� ������
    // � ������� ��� ����� ���������
    SimulationResult runSimulationForDMethod(const DM* dm);
    // ���� �� �������, �� ���� �������� ���� ����������, ������,
    // ����� ������� ���������� �������� t ������� ����,
    // �� t - ��������� ��� � ��� ���� ��������� �������.
    // ���� �������� resourcePtr �� ������� nullptr, ���������� ����
    // ������������ ���� ��� ����� �������.
    // ����� ������� ��� ����������, �� �������� ������� ���
    // ��������� ���������� �����������.
    double waitForRelease(
        TaskAnalizer::AnalizerResult anResult,
        Resource* resourcePtr=nullptr
    );
    // ����� ������� ���������� ��������� ��� �����, �� � �� ��������
    double waitForPerformAllTasks(vector<Resource>& resources);
    // ����� ������� ���������� ��������� ������ � ��������� �������
    // ��������������� � ����� waitForRelease
    double waitForReleaseAvailable(
        TaskAnalizer::AnalizerResult anResult
    );
    // ����� ������� ���������� ��������� ����������� �������
    // ��������������� � ����� waitForRelease
    double waitForReleaseParticular(
        const Task& task,
        Resource& resource
    );
    // ����� ������� ���������� ���� time ��� ������� �������
    void waitForTime(double time, vector<Resource>& resources);
    // ����� ������� �� ������� ���� �����
    void freeAllResources();
    // ����� ������� ���������� ����� � ������� �� ��������.
    // ����������� ��������� � �������� � ����� - ��� ���������� �����.
    // ����� ���� �������� ��� ��������, �������� ���� �������,
    // ��� �� �������� ���� ������, �� �������� �� ������ ���������� �������.
    // ���� ������ �� �������� ���� ������� ����� ������ �������
    // �����, �� ���������� � ����� �������, ����� ���� count.
    double sendTaskToResource(Task& task, Resource& resource);
    // ������ ����-���� ������, ���� ���� �������� ���� ��������
    // �������� ��� �������
    Resource* findAnyFreeResource(
        const Task& task,
        const vector<Resource*>& resources
    );
    // �������� �������� � ����� ��� ���������� ����� �� ��������
    // subTasksCount - ������� ����� ��������, �� ������ �����������
    double getNetworkDelay(
        const Task& task,
        const Resource& res,
        int subTasksCount);
public:
    SimulationEnvironment(DataBase* dataBase);
    ~SimulationEnvironment();
    // ���������� ���������
    struct SimulationResult
    {
        DM::DMethod methodId; // ��� ������ ��������
        double execTime; // ��������� ��� ���������
        double avWaitTime; // ������� ��� ����������
        double idleResPercentage; // ������� ������� �������
    };

    const vector<SimulationResult>& getResults() const;
    void runSimulation();
};