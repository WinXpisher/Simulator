#pragma once
#include <thread>
#include <mutex>
#include <vector>

#include "SimulatorPrimitives.h"
#include "DistributionMethod.h"
#include "TaskAnalizer.h"
#include "SimulationData.h"
#include "Log.h"
using namespace std;

class SimulationEnvironment
{
private:
    using DM = DistributionMethod;

    // ����, �� ���������������� � ���������
    struct SimulationContext
    {
        vector<TaskAnalizer::AnalizerResult> anResults; // ���������� ������ �����
        TaskAnalizer::AnalizerResult anResult; // ��'���, � ����� ���������� ������� ���������
        Resource* assignedResource; // ������, ����������� ������� ��������
        bool hasTask; // �� � �� �������� � ��� �������
        int subTasksRemain; // ������� ����� ��������� ��������, �� ����������
        bool areSubTasksConnected; // �� ���'���� ������ � �������
        double timeUnitToSkip; // ������� ���� ��� ��������
        int waitMilliSec; // ��� ���������� � ����������
        bool actionTaken; // �� ������� �������� �� � ������� ��������
    };
    SimulationContext simContext;

    std::mutex dataBaseMutex;
    Logger* logger;

    struct SendingTask;
    vector<SendingTask> sendingPool; // ��� �������, �� ������������� �� ������
    TaskAnalizer taskAnalizer; // ��'���, ���� ���� ���������� ����� ��������, ����� ����������
    vector<DM*> distributionMethods; // ������ ��������
    DataBase* dataBase; // ���� ����� � ���������� ��������� �� ����������
    
    int channelCount; // ������� ������ ��'����
    double resourceStagnationSum; // �������� ������� ������� �������
    double subTaskWaitingTimeSum; // �������� ��� ���������� �����
    int simulationClock; // ������ ���������
    
    // ���������� ������ ����� ����������
    void initSimContext();
    // ���������� ���������� �������, ��� ���'����� � ����������
    void initTasksSimulationInfo();
    // ����� ������� ���������� ���� time ��� �������
    void waitForResources(double time, vector<Resource>& resources);
    // ����� ������� ���������� ���� time ��� ���� �������� �������
    void waitForSendingPool(double time);
    // �� ���������� �� ��������
    bool haveAllTasksPerformed();
    // ����� ������� ���������� ���� time �� ����������� ���
    void modelWaiting(double time);
    // ����� ������� ���������� ���� time ��� �����, �� ������ � ����
    // � ������� �� ������� waitingTime
    void modelWaitingForSubTasks(double time);
    // �������� �������� ������� �����
    int getSubTasksCount() const;
    // ����� �������� ��������� ��� ���������� ����� �� �������
    // �������� � ������ ��������� � subTaskWaitingTimeSum
    void calcSubTaskWaitingTimeCurrent();
    // ����� ����� �� ��������� ���
    void wait(int time);
    // ����� ��������� �������� �� ������, ���� �� �������.
    // ���� �������� ���� ���������� �������, ����� ���������� ����
    // ������ SENDING � ���� �� ���� ����������. ���� �������� ����
    // ���� ����� ������� ��������, ���� ����������� �� ����� ��������
    // (��� ������ ��������), ���� � ���� ����� �������������� ������
    // SENDING, ������� ������������ �������� �������������� ������ ���
    // DEVIDED, ��� DIVIDED_RUNNING, � ��������� �� ������� �������� ����
    // ����������, �� ����� ������� �� ���� � ��� �������.
    // ����������� ��������� � ���������� ������� ����� ����� ��������.
    int trySendTaskToResource(
        Task& task,
        Resource& resource, 
        bool areSubTasksConnected
    );
    // �������� ���� ��������, ��� �� �������� ������� ����� ����
    // ������ ������� ������� ������� ����� (������� ������ �� ���� ���������). 
    // ��� ������� � �������� ������� �� DIVIDED, ����� ������� ���� ��� ���.
    Task getTaskCopyWithRemainingCount(const Task& task);
    // ����� ���������� ������ ���������� �������� � ��������� �� ����,
    // �� ������� �� ������ ����� �������� ��������� �� ������� (�����
    // ���� ���� �� ����������� � ��� ����������). ���� �� ���, �� ��������������
    // ������ DIVIDED_RUNNING, ������ - ������ DIVIDED
    void setDividedTaskStatus(Task& parentTask);
    // ����� ������� ���������� �������� (���� ���� �) ��������� ��������, ����
    // ��� ����� �� ������ DIVIDED_RUNNING � �� ���� ������ �������� �����
    // ������ PERFORMED
    void finishDividedTaskIfNeed(Task& childTask);
    // �������� �������� � ����� ��� ���������� ����� �� ��������
    // subTasksCount - ������� ����� ��������, �� ������ �����������
    double getNetworkDelay(
        const Task& task,
        const Resource& res,
        int subTasksCount);
public:
    // ��������, ��� ������������� �� ������
    struct SendingTask
    {
        Task* task;
        float timeToSend; // ���, ���� ���������, ��� ��������� ��������
    };

    SimulationEnvironment(DataBase* dataBase, Logger* logger);
    ~SimulationEnvironment();

    // ����� ���������� ��� ��� ������� ���������
    void prepareForSimulation(
        int channelCount=1,
        int timeUnitToSkip=1,
        int waitMilliSec=400
    );
    // ����� ������� ��������� ��� ������ ����������� ������
    void runSimulation(const DM* dm);
    
    int getSimulationClock() const { return simulationClock; }
    const DataBase& getDataBase() const { return *dataBase; }
    std::mutex& getDataBaseMutex() { return dataBaseMutex; }
    const vector<SendingTask>& getSendingPool() const { return sendingPool; }
    // �������� ������� ������� ������� �������
    const double getResourceStagnationAv() const
    { 
        return (simulationClock == 0) ? 0 :
            resourceStagnationSum / simulationClock;
    }

    // �������� ������� ��� ���������� �����
    const double getSubTaskWaitingTimeAv() const
    {
        int subTasksCount = getSubTasksCount();
        return (subTasksCount == 0) ? 0 :
            subTaskWaitingTimeSum / subTasksCount;
    }
};