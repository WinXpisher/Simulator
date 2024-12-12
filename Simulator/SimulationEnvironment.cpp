#include "SimulationEnvironment.h"

using DM = DistributionMethod;
SimulationEnvironment::SimulationEnvironment(DataBase* dataBase):
    dataBase(dataBase), taskAnalizer(dataBase)
{

}

SimulationEnvironment::~SimulationEnvironment()
{
    delete dataBase;
    dataBase = nullptr;
    for (auto dm : distributionMethods)
        delete dm;
}

void SimulationEnvironment::initDMethods()
{
    for (auto dm : DM::dMethods)
    {
        switch (dm)
        {
        case DM::FCFS:
            break;
        case DM::LIFO:
            distributionMethods.push_back(new LIFO());
            break;
        case DM::HPF:
            break;
        case DM::BACKFILL:
            break;
        case DM::SIMPLEX:
            break;
        case DM::SMART:
            break;
        default:
            break;
        }
    }
}

void SimulationEnvironment::runSimulation()
{
    // ��������� �� ������� ������ �����������
    for (DM* dm : distributionMethods)
    {
        //runResultsForDMethod(dm);
    }
}

void SimulationEnvironment::runResultsForDMethod(const DM* dm)
{
    /*vector<Task> tasksCopy = tasks;
    Task task;
    while (dm->nextTask(tasks, task))
    {
        
    }*/
}

void TaskAnalizer::analizeAllTasks()
{
    // ��������� �����, ��� �������� ���������� ��� ������� ��������
    // ������� ���������� ������ ������� � ���� conResult
    analizeConnectivity();
    for (ConnectivityResult& cr : conResult)
    {
        analizeTask(*cr.taskPtr, cr.areConnected);
    }
}

void TaskAnalizer::analizeConnectivity()
{
    bool isConnected;
    for (Task& task : dataBase->tasks)
    {
        // ���� ���������� ��'������ ����� 0.3, ������ ���������� ��'�������
        if (task.connectivity > 0.3)
            isConnected = true;
        else isConnected = false;
        // ������� ������������ ������
        conResult.push_back({ &task, isConnected });
    }
}

void TaskAnalizer::analizeTask(Task& task, bool isConnected)
{
    bool wasFound = false; // �� ��� ��������� ���� � ���� ������
    AnalizerResult anRes;
    anRes.taskPtr = &task;
    for (Resource& res : dataBase->availableResources)
    {
        // ���� �������� �� ���� ���� ��������, ���������� ��������
        // � ��������� �� � ������ ���'���� �� � ������������ ��� ������ �������� 
        if (isConnected && !canBePerformedConnected(task, res))
            continue;
        if (!isConnected && !canBePerformedSimple(task, res))
            continue;

        wasFound = true;
        // ������ �������� �� ������, �� ����� �������� ���� ���� ��������
        anRes.resources.push_back(&res);
    }
    // ���� ��� �������� ���� ��� ���� ������, �� ����� ���� ���� ����������
    // ������������ ������ WAITING
    if (wasFound)
        task.status = Task::TaskStatus::WAITING;
    // ������ ������������ ������ CANCELLED
    else
        task.status = Task::TaskStatus::CANCELLED;
    // ������ ��������� �� ���������� ������� ����������
    anResult.push_back(anRes);
}

bool TaskAnalizer::canBePerformedSimple(Task& task, Resource& res)
{
    // ���������� �������� ���������� ���������
    if (task.resDesc.procArch != res.resDesc.procArch)
        return false;

    // ���������� �������� ����������� ������
    if (task.resDesc.os != res.resDesc.os)
        return false;

    // ���� �������� ��������� �� ��������, �������� ���������� �� ����
    if (task.resDesc.procSpeed > res.resDesc.procSpeed)
        return false;

    // ���� ���������� ���'�� �� ���������, �������� ���������� �� ����
    if (task.resDesc.memSize > res.resDesc.memSize)
        return false;

    // ���� ���� �� ����� �� ���������, �������� ���������� �� ����
    if (task.resDesc.discSize > res.resDesc.discSize)
        return false;

    // ���� �� �������� ������� ������, �������� ���� ���� ��������
    return true;
}

bool TaskAnalizer::canBePerformedConnected(Task& task, Resource& res)
{
    // ���� ��������� �� ���������, �������� ���������� �� ����
    if (task.resDesc.procCount > res.resDesc.procCount)
        return false;

    return canBePerformedSimple(task, res);
}