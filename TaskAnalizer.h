#pragma once
#include <vector>
#include "SimulatorPrimitives.h"
using namespace std;
// ����� ������ ������, �� �������� ��������� ������� �� ����
// ���� ���������� ����� ������
class TaskAnalizer
{
public:
    struct AnalizerResult
    {
        Task* task; // ���� ��������
        vector<Resource*> resources; // �������, �� ���� �������� ���� ���� ��������
    };
private:
    struct ConnectivityResult
    {
        Task* task; // ���� ��������
        bool areConnected; // �� � ������ � ������� ��'�������
    };
    // ��������� ����������� ������ analizeConnectivity
    vector<ConnectivityResult> conResult;
    // ��������� ����������� ������ analizeAllTasks
    vector<AnalizerResult> anResult;
    // �� ���� �� anResult, ��� ��� ������� � �������� Cancelled
    // ���� ������������ ������� ��������
    vector<AnalizerResult> anResultClear;
    DataBase* dataBase; // ���� ����� � ���������� ��������� �� ����������
    // ����� ������ �������� ����� � ������� �������
    // �� �������� ������ conResult
    void analizeConnectivity();
    // ����� ������ ��������
    void analizeTask(Task& task, bool isConnected);
    // �� ���� �������� � �� ��'������� �������� ���� �������� �� ����������� ������
    // ����� �� ������� ������� ���������, ���� �� �� ���'���� ������
    // ������ ���� ��������� �� ������� ��������
    bool canBePerformedSimple(const Task& task, Resource& res);
    // �� ���� �������� � ��'������� �������� ���� �������� �� ����������� ������
    // ����� ������� ������� ���������, ���� �� ���'���� ������
    // �� ������ ���� ��������� �� ������� ��������
    bool canBePerformedConnected(const Task& task, Resource& res);
    // ����� ��������� �� ������� � �������� Cancelled
    // ��������� ���������� � anResultClear
    // ����� ����������� ����������� � ����� analizeAllTasks
    void clearFromCancelled();
public:
    TaskAnalizer(DataBase* dataBase) : dataBase(dataBase) {}
    // ����� ������� �� �������, �� ���� �������� ���� ����������
    // �� ���������� ������ Waiting, ���� ����� ������� ����
    // ����� ���������� ������ Cancelled
    void analizeAllTasks();
    // �� ���'���� ������ � �������
    bool areSubTasksConnected(const Task& task);
    vector<AnalizerResult>& getAnalizeResult() { return anResult; }
    vector<AnalizerResult>& getAnalizeResultClear() { return anResultClear; }
};