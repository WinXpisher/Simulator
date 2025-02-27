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

    // змінні, які використовуються в симуляції
    struct SimulationContext
    {
        vector<TaskAnalizer::AnalizerResult> anResults; // результати аналізу задач
        TaskAnalizer::AnalizerResult anResult; // об'єкт, в якому зберігається поточне завадання
        Resource* assignedResource; // ресурс, призначений методом розподілу
        bool hasTask; // чи є ще завдання в пулі завдань
        int subTasksRemain; // кількість задач поточного завдання, що залишились
        bool areSubTasksConnected; // чи пов'язані задачі в завданні
        double timeUnitToSkip; // одиниця часу для пропуску
        int waitMilliSec; // час очікування в мілісекундах
        bool actionTaken; // чи вдалося виконати дію в поточній ітерації
    };
    SimulationContext simContext;

    std::mutex dataBaseMutex;
    Logger* logger;

    struct SendingTask;
    vector<SendingTask> sendingPool; // пул завдань, які відправляються на ресурс
    TaskAnalizer taskAnalizer; // об'єкт, який буде аналізувати кожне завдання, перед симуляцією
    vector<DM*> distributionMethods; // методи розподілу
    DataBase* dataBase; // база даних к доступними ресурсами та завданнями
    
    int channelCount; // кількість каналів зв'язку
    double resourceStagnationSum; // сумарний процент простою ресурсів
    double subTaskWaitingTimeSum; // сумарний час очікування задач
    int simulationClock; // таймер симуляції
    
    // ініцалізація змінних перед симуляцією
    void initSimContext();
    // ініцалізація інформації завдань, яка пов'язана з симуляцією
    void initTasksSimulationInfo();
    // метод моделює очікування часу time для ресурсів
    void waitForResources(double time, vector<Resource>& resources);
    // метод моделює очікування часу time для пулу відправки завдань
    void waitForSendingPool(double time);
    // чи виконалися всі завдання
    bool haveAllTasksPerformed();
    // метод моделює очікування часу time на програмному рівні
    void modelWaiting(double time);
    // метод моделює очікування часу time для задач, які стоять в черзі
    // і оновлює їх атрибут waitingTime
    void modelWaitingForSubTasks(double time);
    // отримати загальну кількість задач
    int getSubTasksCount() const;
    // метод підраховує загальний час очікування задач на поточній
    // ітерації і зберігає результат в subTaskWaitingTimeSum
    void calcSubTaskWaitingTimeCurrent();
    // метод очікує на фізичному рівні
    void wait(int time);
    // Метод розподіляє завдання на ресурс, якщо це можливо.
    // Якщо завдання було розподілено повністю, метод встановлює йому
    // статус SENDING і додає до пулу надсилання. Якщо надіслана може
    // бути тільки частина завдання, воно розділяється на менші завдання
    // (або дочірні завдання), яким в свою чергу встановлюється статус
    // SENDING, причому батьківському завданню встановлюється статус або
    // DEVIDED, або DIVIDED_RUNNING, в залежності чи повністю завдання було
    // розподілено, чи якась частина ще чекає в пулі завдань.
    // Повертаючим значенням є розподілена кількість задач цього завдання.
    int trySendTaskToResource(
        Task& task,
        Resource& resource, 
        bool areSubTasksConnected
    );
    // Отримати копію завдання, але від загальної кількості задач буде
    // віднята сумарна кількість дочірніх задач (поточна задача має бути розділеною). 
    // Для завдань зі статусом відмінним від DIVIDED, метод повертає копію без змін.
    Task getTaskCopyWithRemainingCount(const Task& task);
    // Метод встановлює статус розділеного завдання в залежності від того,
    // чи повністю всі задачі цього завдання розподілені на ресурси (навіть
    // якщо вони ще знаходяться в пулі надсилання). Якщо це так, то встановлюється
    // статус DIVIDED_RUNNING, інакше - просто DIVIDED
    void setDividedTaskStatus(Task& parentTask);
    // Метод завершує батьківське завдання (якщо таке є) поточного завдання, якщо
    // сам метод має статус DIVIDED_RUNNING і всі його дочірні завдання мають
    // статус PERFORMED
    void finishDividedTaskIfNeed(Task& childTask);
    // отримати затримку в мережі при надходженні задач до кластеру
    // subTasksCount - кількість задач завдання, які будуть надсилатися
    double getNetworkDelay(
        const Task& task,
        const Resource& res,
        int subTasksCount);
public:
    // завдання, яке відправляється на ресурс
    struct SendingTask
    {
        Task* task;
        float timeToSend; // час, який залишився, щоб відправити завдання
    };

    SimulationEnvironment(DataBase* dataBase, Logger* logger);
    ~SimulationEnvironment();

    // метод підготовлює дані для початку симуляції
    void prepareForSimulation(
        int channelCount=1,
        int timeUnitToSkip=1,
        int waitMilliSec=400
    );
    // метод запускає симуляцію для одного конкретного методу
    void runSimulation(const DM* dm);
    
    int getSimulationClock() const { return simulationClock; }
    const DataBase& getDataBase() const { return *dataBase; }
    std::mutex& getDataBaseMutex() { return dataBaseMutex; }
    const vector<SendingTask>& getSendingPool() const { return sendingPool; }
    // отримати середній процент простою ресурсів
    const double getResourceStagnationAv() const
    { 
        return (simulationClock == 0) ? 0 :
            resourceStagnationSum / simulationClock;
    }

    // отримати середній час очікування задач
    const double getSubTaskWaitingTimeAv() const
    {
        int subTasksCount = getSubTasksCount();
        return (subTasksCount == 0) ? 0 :
            subTaskWaitingTimeSum / subTasksCount;
    }
};