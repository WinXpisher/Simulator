#pragma once
#include <vector>
#include "SimulatorPrimitives.h"
using namespace std;
// класс аналізує задачі, та допомагає визначити ресурси на яких
// може виконатися кожна задача
class TaskAnalizer
{
public:
    struct AnalizerResult
    {
        Task* task; // саме завдання
        vector<Resource*> resources; // ресурси, на яких завдання може бути виконано
    };
private:
    struct ConnectivityResult
    {
        Task* task; // саме завдання
        bool areConnected; // чи є задачі в завданні зв'язаними
    };
    // результат відрацювання методу analizeConnectivity
    vector<ConnectivityResult> conResult;
    // результат відрацювання методу analizeAllTasks
    vector<AnalizerResult> anResult;
    // те саме що anResult, але без завдань зі статусом Cancelled
    // буде передаватися методам розподілу
    vector<AnalizerResult> anResultClear;
    DataBase* dataBase; // база даних з доступними ресурсами та завданнями
    // метод аналізує звязність задач в кожному завданні
    // та заповнює вектор conResult
    void analizeConnectivity();
    // метод аналізує завдання
    void analizeTask(Task& task, bool isConnected);
    // чи може завдання з НЕ зв'язаними задачами бути виконано на конкретному ресурсі
    // метод не враховує кількість процесорів, тому що не пов'язані задачі
    // можуть бути розподілені на декілька кластерів
    bool canBePerformedSimple(const Task& task, Resource& res);
    // чи може завдання з зв'язаними задачами бути виконано на конкретному ресурсі
    // метод враховує кількість процесорів, тому що пов'язані задачі
    // НЕ можуть бути розподілені на декілька кластерів
    bool canBePerformedConnected(const Task& task, Resource& res);
    // очищує результат від завдань зі статусом Cancelled
    // результат записується в anResultClear
    // метод викликається автоматично в методі analizeAllTasks
    void clearFromCancelled();
public:
    TaskAnalizer(DataBase* dataBase) : dataBase(dataBase) {}
    // метод визначає всі ресурси, на яких завдання може виконатися
    // та встановлює статус Waiting, якщо таких ресурсів немає
    // метод встановлює статус Cancelled
    void analizeAllTasks();
    // чи пов'язані задачі в завданні
    bool areSubTasksConnected(const Task& task);
    vector<AnalizerResult>& getAnalizeResult() { return anResult; }
    vector<AnalizerResult>& getAnalizeResultClear() { return anResultClear; }
};