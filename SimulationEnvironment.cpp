#include <algorithm> // для std::min_element
#include "SimulationEnvironment.h"
#include "ResourceManager.h"

using DM = DistributionMethod;
SimulationEnvironment::SimulationEnvironment(DataBase* dataBase):
    dataBase(dataBase), taskAnalizer(dataBase)
{
    initDMethods();
}

SimulationEnvironment::~SimulationEnvironment()
{
    for (DM* dmPtr : distributionMethods)
    {
        delete dmPtr;
    }
}

void SimulationEnvironment::initDMethods()
{
    for (auto dm : DM::dMethods)
    {
        switch (dm)
        {
        case DM::FCFS:
            distributionMethods.push_back(new FCFS());
            break;
        case DM::LIFO:
            distributionMethods.push_back(new LIFO());
            break;
        case DM::HPF:
            distributionMethods.push_back(new HPF());
            break;
        case DM::BACKFILL:
            break;
        case DM::SIMPLEX:
            distributionMethods.push_back(new SIMPLEX());
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
    taskAnalizer.analizeAllTasks();
    simResults.clear();
    // проходимо по кожному методу розподілення
    for (DM* dm : distributionMethods)
    {
        SimulationResult simRes = runSimulationForDMethod(dm);
        simResults.push_back(simRes);
    }
}

const vector<SimulationEnvironment::SimulationResult>&
    SimulationEnvironment::getResults() const
{
    return simResults;
}

SimulationEnvironment::SimulationResult SimulationEnvironment::runSimulationForDMethod(
    const DM* dm
)
{
    SimulationResult simResult {dm->getId(), 0, 0, 0};
    // кількість очікувань (використовується для рохрахунку середнього часу очікування)
    int waitingCount = 0;
    double waitingTime = 0; // загальний час очікування
    // отримуємо результати аналізу задач, які не мають статусу CANCELLED
    vector<TaskAnalizer::AnalizerResult> anResultsCopy = 
        taskAnalizer.getAnalizeResultClear();
    TaskAnalizer::AnalizerResult anResult;
    Resource* resource=nullptr;
    while (dm->nextTask(anResultsCopy, anResult, resource))
    {
        // якщо метод розподілу не повернув ресурс
        if (resource == nullptr)
        {
            while (true)
            {
                // очікуємо на звільнення будь-якого ресурсу, який звільниться найшвидше
                waitingTime += waitForRelease(anResult);
                ++waitingCount;
                // знаходимо такий ресурс
                resource = findAnyFreeResource(anResult.task, anResult.resources);
                // надсилаємо завдання на виконання, метод змінює поле task.count,
                // яке говорить скільки задач ще залишилося розподілити
                simResult.execTime += sendTaskToResource(anResult.task, *resource);
                // якщо завдання повністю розподілено, завершуємо цикл
                if (anResult.task.count <= 0)
                    break;
            }
        }
        else
        {
            while (true)
            {
                // очікуємо на звільнення заданого ресурсу
                waitingTime += waitForRelease(anResult, resource);
                ++waitingCount;
                // надсилаємо завдання на виконання, метод змінює поле task.count,
                // яке говорить скільки задач ще залишилося розподілити
                simResult.execTime += sendTaskToResource(anResult.task, *resource);
                // якщо завдання повністю розподілено, завершуємо цикл
                if (anResult.task.count <= 0)
                    break;
            }
        }
        resource = nullptr;
    }
    // чекаємо на виконання всіх задач
    simResult.execTime += waitForPerformAllTasks(dataBase->availableResources);
    // додаємо до загального часу виконання загальний час очікування
    simResult.execTime += waitingTime;
    // розраховуємо середнє значення очікування
    simResult.avWaitTime = waitingTime / waitingCount;
    return simResult;
}

Resource* SimulationEnvironment::findAnyFreeResource(
    const Task& task,
    const vector<Resource*>& resources
)
{
    bool areSubTasksConnected = taskAnalizer.areSubTasksConnected(task);
    for (Resource* resource : resources)
    {
        // якщо задачі не зв'язані між собою і хоча б одна задача може
        // виконатися,повертаємо цей ресурс
        if (!areSubTasksConnected &&
            ResourceManager::howManyTasksCanBePerformed(task, *resource) > 0
            ) return resource;
        // якщо задачі зв'язані між собою ресурс повинен мати
        // змогу виконати їх всі одразу
        else if (areSubTasksConnected &&
            ResourceManager::howManyTasksCanBePerformed(task, *resource) >= task.count
            ) return resource;
    }
    return nullptr;
}

double SimulationEnvironment::waitForPerformAllTasks(vector<Resource>& resources)
{
    double maxTimeToWait = 0; // час, потрібний для виконання всіх завдань
    // проходимо по кожному ресурсу
    for (Resource& res : resources)
    {
        // проходимо по кожному завданню, що виконується на поточному ресурсі
        for (Task& task : res.performingTasks)
        {
            // шакаємо найбільше значення для гарантії, що всі завдання
            // будуть виконані
            if (task.performTime > maxTimeToWait)
                maxTimeToWait = task.performTime;
        }
    }
    waitForTime(maxTimeToWait, resources); // моделюємо очікування
    return maxTimeToWait; // повертаємо час очікування
}

double SimulationEnvironment::waitForRelease(
    TaskAnalizer::AnalizerResult anResult,
    Resource* resourcePtr
)
{
    double waitingTime; // час очікування
    // якщо ресурс задано, чекаємо на його звільнення
    if (resourcePtr != nullptr)
    {
        waitingTime = waitForReleaseParticular(
            anResult.task,
            *resourcePtr
        );
    }
    // інакше обираємо для цього ресурс, який звільниться найшвидше
    else
    {
        waitingTime = waitForReleaseAvailable(anResult);
    }
    return waitingTime;
}

double SimulationEnvironment::waitForReleaseAvailable(
    TaskAnalizer::AnalizerResult anResult
)
{
    // чи є задачі зв'язаними
    bool areSubTasksConnected = taskAnalizer.areSubTasksConnected(anResult.task);
    //cout << "canBePerformedSimple: " << taskAnalizer.canBePerformedConnected(
    //    anResult.task, *anResult.resources[0]
    //) << endl;
    double waitingTime = 0; // час очікування
    double minTimeToWait;
    while (true)
    {
        // перевіряємо чи є якийсь ресурс доступним
        bool resourceAvailable = std::any_of(
            anResult.resources.begin(),
            anResult.resources.end(),
            [anResult, areSubTasksConnected](const Resource* resource) {
                // якщо задачі пов'язані, то ресурс треба звільнити так,
                // щоб завдання могло повністю виконатися за один раз
                if (areSubTasksConnected)
                    return ResourceManager::howManyTasksCanBePerformed(
                        anResult.task,
                        *resource
                    ) >= anResult.task.count;
                // інакше достатньо можливості виконання будь-якої кількості
                // задач на ресурсі
                else
                    return ResourceManager::howManyTasksCanBePerformed(
                        anResult.task,
                        *resource
                    ) > 0;
            }
        );

        // якщо так, виходимо з циклу
        if (resourceAvailable)
            break;

        // інакше шукаємо ресурс, який найскоріше за всіх виконає якусь задачу
        auto minIter = std::min_element(
            anResult.resources.begin(),
            anResult.resources.end(),
            [](const Resource* r1, const Resource* r2) {
                return ResourceManager::getMinTimeToFree(*r1) <
                    ResourceManager::getMinTimeToFree(*r2);
            }
        );
        Resource* minResource = *minIter;
        // отримуємо мінімальний час для звільнення ресурсу
        minTimeToWait = ResourceManager::getMinTimeToFree(*minResource);
        // моделюємо очікування
        waitForTime(minTimeToWait, dataBase->availableResources);
        waitingTime += minTimeToWait;
    }
    return waitingTime;
}

double SimulationEnvironment::waitForReleaseParticular(
    const Task& task,
    Resource& resource
)
{
    // чи є задачі зв'язаними
    bool areSubTasksConnected = taskAnalizer.areSubTasksConnected(task);
    double waitingTime = 0; // час очікування
    double minTimeToWait;
    // якщо задачі пов'язані, цикл продовжується до тих пір, доки якогось
    // ресурсу не буде достатньо для виконання всього завдання, інакше -
    // достатньо можливості виконання будь-якої кількості задач із завдання
    while (
        (areSubTasksConnected &&
            ResourceManager::howManyTasksCanBePerformed(task, resource) < task.count) ||
        (!areSubTasksConnected && 
            ResourceManager::howManyTasksCanBePerformed(task, resource) == 0
        ))
    {
        // чекаємо виконання завдань, яким залишилося найменше часу
        minTimeToWait = ResourceManager::getMinTimeToFree(resource);
        waitForTime(minTimeToWait, dataBase->availableResources);
        waitingTime += minTimeToWait;
    }
    return waitingTime;
}

void SimulationEnvironment::waitForTime(
    double time,
    vector<Resource>& resources
)
{
    // проходимо по кожному ресурсу
    for (Resource& res : resources)
    {
        // проходимо по кожному завданню, що виконується на поточному ресурсі.
        // Використовуємо ітератор, щоб видалять завдання, у яких час, що
        // залишився до кінця їх виконання, менше або дорівнює нулю
        for (
            auto iter = res.performingTasks.begin();
            iter != res.performingTasks.end();
            )
        {
            iter->performTime -= time; // віднімаємо час, який мав пройти
            // якщо завдання вже було виконано, видаляємо його
            if (iter->performTime <= 0)
                iter = res.performingTasks.erase(iter);
            else
                ++iter;
        }
    }
}

void SimulationEnvironment::freeAllResources()
{
    for (Resource& resource : dataBase->availableResources)
        resource.performingTasks.clear();
}

double SimulationEnvironment::sendTaskToResource(
    Task& task,
    Resource& resource
)
{
    double sentTime = 0; // затримка надсилання
    // скільки таких задач може виконатися на ресурсі
    int canBePerformed = ResourceManager::howManyTasksCanBePerformed(
        task,
        resource
    );
    // скільки задач буде виконано
    int willBePerformed = (canBePerformed > task.count)?
        task.count : canBePerformed;

    // створюємо копію завдання, яка буде додаватися в ресурс
    Task taskCopy = task;
    // додаватися буде не обов'язково вся задача, а тільки та 
    // кількість задач, для якої в ресурсі залишилось місце
    taskCopy.count = willBePerformed;

    // додаємо завдання до вектору завдань, які зараз виконуються на ресурсі
    resource.performingTasks.push_back(taskCopy);
    // зменшуємо кількість задач, які ще залишилось розподілити
    task.count -= willBePerformed;
    // повертаємо загальну затримку при надсиланні задач до кластеру
    return getNetworkDelay(task, resource, willBePerformed);
}

double SimulationEnvironment::getNetworkDelay(
    const Task& task,
    const Resource& res,
    int subTasksCount
)
{
    // ділимо розмір задачі на пропускну здатність каналу size/(size/time)
    // і отримуємо час, за який відправиться одна задача
    double delayPerSubTask = task.subTaskSize / res.bandwidth;
    return delayPerSubTask * subTasksCount;
}