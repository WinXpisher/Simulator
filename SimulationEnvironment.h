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

    TaskAnalizer taskAnalizer; // об'Їкт, €кий буде анал≥зувати кожне завданн€, перед симул€ц≥Їю
    vector<DM*> distributionMethods; // методи розпод≥лу
    DataBase* dataBase; // база даних к доступними ресурсами та завданн€ми
    vector<SimulationResult> simResults; // наб≥р результат≥в дл€ кожного методу розпод≥лу

    // метод ≥н≥ц≥ал≥зуЇ вектор метод≥в розпод≥лу
    void initDMethods();
    // метод запускаЇ симул€ц≥ю дл€ одного конкретного методу
    void runResultsForDMethod(const DM* dm);
    // €кщо вс≥ ресурси, на €ких завданн€ може виконатис€ зайн€т≥,
    // метод моделюЇ оч≥куванн€ прот€гом t одиниць часу.
    // де t - м≥н≥мальний час з ус≥х час≥в зв≥льненн€ ресурс≥в
    void waitForRelease();
public:
    SimulationEnvironment(DataBase* dataBase);
    ~SimulationEnvironment();
    // результати симул€ц≥њ
    struct SimulationResult
    {
        DM::DMethod methodId; // айд≥ методу розпод≥лу
        double execTime; // загальний час виконанн€
        double avWaitTime; // середн≥й час оч≥куванн€
        double idleResPercentage; // в≥дсоток простою ресурс≥в
    };

    void runSimulation();
};