#pragma once
#include <vector>
#include "SimulatorPrimitives.h"
#include "DistributionMethod.h"

using namespace std;
// ����� ������ ������, �� �������� ��������� ������� �� ����
// ���� ���������� ����� ������
class TaskAnalizer
{
private:
    struct ConnectivityResult
    {
        Task* taskPtr; // �������� �� ���� ��������
        bool areConnected; // �� � ������ � ������� ��'�������
    };
    struct AnalizerResult
    {
        Task* taskPtr; // �������� �� ���� ��������
        vector<Resource*> resources; // �������, �� ���� �������� ���� ���� ��������
    };
    // ��������� ����������� ������ analizeConnectivity
    vector<ConnectivityResult> conResult;
    // ��������� ����������� ������ analizeTasks
    vector<AnalizerResult> anResult;
    DataBase* dataBase; // ���� ����� � ���������� ��������� �� ����������
    // ����� ������ �������� ����� � ������� �������
    // �� �������� ������ conResult
    void analizeConnectivity();
    // ����� ������ ��������
    void analizeTask(Task& task, bool isConnected);
    // �� ���� �������� � �� ��'������� �������� ���� �������� �� ����������� ������
    // ����� �� ������� ������� ���������, ���� �� �� ���'���� ������
    // ������ ���� ��������� �� ������� ��������
    bool canBePerformedSimple(Task& task, Resource& res);
    // �� ���� �������� � ��'������� �������� ���� �������� �� ����������� ������
    // ����� ������� ������� ���������, ���� �� ���'���� ������
    // �� ������ ���� ��������� �� ������� ��������
    bool canBePerformedConnected(Task& task, Resource& res);
public:
    TaskAnalizer(DataBase* dataBase) : dataBase(dataBase) {}
    // ����� ������� �� �������, �� ���� �������� ���� ����������
    // �� ���������� ������ Waiting, ���� ����� ������� ����
    // ����� ���������� ������ Cancelled
    void analizeAllTasks();
    vector<AnalizerResult>& getAnalizeResult() { return anResult; }
};

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