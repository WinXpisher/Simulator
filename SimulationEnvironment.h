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
    // ≥ повертаЇ дл€ нього результат
    SimulationResult runSimulationForDMethod(const DM* dm);
    // якщо вс≥ ресурси, на €ких завданн€ може виконатис€, зайн€т≥,
    // метод моделюЇ оч≥куванн€ прот€гом t одиниць часу,
    // де t - м≥н≥мальний час з ус≥х час≥в зв≥льненн€ ресурс≥в.
    // якщо параметр resourcePtr не дор≥внюЇ nullptr, оч≥куванн€ буде
    // виконуватис€ саме дл€ цього ресурсу.
    // ћетод повертаЇ час оч≥куванн€, це значенн€ потр≥бно дл€
    // отриманн€ результат≥в моделюванн€.
    double waitForRelease(
        TaskAnalizer::AnalizerResult anResult,
        Resource* resourcePtr=nullptr
    );
    // метод моделюЇ оч≥куванн€ виконанн€ вс≥х задач, €к≥ Ї на ресурсах
    double waitForPerformAllTasks(vector<Resource>& resources);
    // метод моделюЇ оч≥куванн€ зв≥льненн€ одного з доступних ресурс≥в
    // використовуЇтьс€ в метод≥ waitForRelease
    double waitForReleaseAvailable(
        TaskAnalizer::AnalizerResult anResult
    );
    // метод моделюЇ оч≥куванн€ зв≥льненн€ конкретного ресурсу
    // використовуЇтьс€ в метод≥ waitForRelease
    double waitForReleaseParticular(
        const Task& task,
        Resource& resource
    );
    // метод моделюЇ оч≥куванн€ часу time дл€ кожного ресурсу
    void waitForTime(double time, vector<Resource>& resources);
    // метод зв≥льнюЇ вс≥ ресурси бази даних
    void freeAllResources();
    // ћетод моделюЇ надсиланн€ задач в завданн≥ до кластеру.
    // ѕовертаючим значенн€м Ї затримка в мереж≥ - час надсиланн€ задач.
    // ћетод може над≥слати все завданн€, над≥слати його частину,
    // або не над≥слати його взагал≥, це залежить в≥д в≥льних компонент≥в ресурсу.
    // якщо задач≥ ≥з завданн€ були над≥слан≥ метод зменшуЇ к≥льк≥сть
    // задач, €к≥ залишилис€ в цьому завданн≥, тобто поле count.
    double sendTaskToResource(Task& task, Resource& resource);
    // знайти будь-€кий ресурс, €кий може виконати дане завданн€
    // частково або повн≥стю
    Resource* findAnyFreeResource(
        const Task& task,
        const vector<Resource*>& resources
    );
    // отримати затримку в мереж≥ при надходженн≥ задач до кластеру
    // subTasksCount - к≥льк≥сть задач завданн€, €к≥ будуть надсилатис€
    double getNetworkDelay(
        const Task& task,
        const Resource& res,
        int subTasksCount);
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

    const vector<SimulationResult>& getResults() const;
    void runSimulation();
};