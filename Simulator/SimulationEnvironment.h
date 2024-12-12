#pragma once
#include <vector>
#include "SimulatorPrimitives.h"
#include "DistributionMethod.h"

using namespace std;
// класс аналізує задачі, та допомагає визначити ресурси на яких
// може виконатися кожна задача
class TaskAnalizer
{
private:
    struct ConnectivityResult
    {
        Task* taskPtr; // покажчик на саме завдання
        bool areConnected; // чи є задачі в завданні зв'язаними
    };
    struct AnalizerResult
    {
        Task* taskPtr; // покажчик на саме завдання
        vector<Resource*> resources; // ресурси, на яких завдання може бути виконано
    };
    // результат відрацювання методу analizeConnectivity
    vector<ConnectivityResult> conResult;
    // результат відрацювання методу analizeTasks
    vector<AnalizerResult> anResult;
    DataBase* dataBase; // база даних з доступними ресурсами та завданнями
    // метод аналізує звязність задач в кожному завданні
    // та заповнює вектор conResult
    void analizeConnectivity();
    // метод аналізує завдання
    void analizeTask(Task& task, bool isConnected);
    // чи може завдання з НЕ зв'язаними задачами бути виконано на конкретному ресурсі
    // метод не враховує кількість процесорів, тому що не пов'язані задачі
    // можуть бути розподілені на декілька кластерів
    bool canBePerformedSimple(Task& task, Resource& res);
    // чи може завдання з зв'язаними задачами бути виконано на конкретному ресурсі
    // метод враховує кількість процесорів, тому що пов'язані задачі
    // НЕ можуть бути розподілені на декілька кластерів
    bool canBePerformedConnected(Task& task, Resource& res);
public:
    TaskAnalizer(DataBase* dataBase) : dataBase(dataBase) {}
    // метод визначає всі ресурси, на яких завдання може виконатися
    // та встановлює статус Waiting, якщо таких ресурсів немає
    // метод встановлює статус Cancelled
    void analizeAllTasks();
    vector<AnalizerResult>& getAnalizeResult() { return anResult; }
};

class SimulationEnvironment
{
private:
    using DM = DistributionMethod;
    struct SimulationResult;

    TaskAnalizer taskAnalizer; // об'єкт, який буде аналізувати кожне завдання, перед симуляцією
    vector<DM*> distributionMethods; // методи розподілу
    DataBase* dataBase; // база даних к доступними ресурсами та завданнями
    vector<SimulationResult> simResults; // набір результатів для кожного методу розподілу

    // метод ініціалізує вектор методів розподілу
    void initDMethods();
    // метод запускає симуляцію для одного конкретного методу
    void runResultsForDMethod(const DM* dm);
public:
    SimulationEnvironment(DataBase* dataBase);
    ~SimulationEnvironment();
    // результати симуляції
    struct SimulationResult
    {
        DM::DMethod methodId; // айді методу розподілу
        double execTime; // загальний час виконання
        double avWaitTime; // середній час очікування
        double idleResPercentage; // відсоток простою ресурсів
    };

    void runSimulation();
};