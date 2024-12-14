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
    void runResultsForDMethod(const DM* dm);
    // ���� �� �������, �� ���� �������� ���� ���������� ������,
    // ����� ������� ���������� �������� t ������� ����.
    // �� t - ��������� ��� � ��� ���� ��������� �������
    void waitForRelease();
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

    void runSimulation();
};