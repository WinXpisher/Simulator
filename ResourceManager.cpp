#include "ResourceManager.h"
using RM = ResourceManager;

RM::ResourceRemaining RM::getResourceRemainingData(
    const Resource& res,
    bool considerStatus,
    Task::TaskStatus tStatus
)
{
    RM::ResourceRemaining resRem;
    // спочатку присвоюємо загальні параметри ресурсу
    // не враховуючи частину, яка є зайнятою
    resRem.discSize = res.resDesc.discSize;
    resRem.memSize = res.resDesc.memSize;
    resRem.procCount = res.resDesc.procCount;

    // віднімаємо від параметрів ресурсу параметри, 
    // зайняті кожною задачею
    for (const auto& task : res.performingTasks)
    {
        // якщо треба враховувати статус завдань і він не
        // співпадає, пропускаємо ітерацію
        if (considerStatus && task->status != tStatus)
            continue;
        // множимо кожен параметр на кількість задач із
        // завдання, які виконуються в даний момент
        resRem.discSize -= task->resDesc.discSize * task->count;
        resRem.memSize -= task->resDesc.memSize * task->count;
        resRem.procCount -= task->resDesc.procCount * task->count;
    }

    return resRem;
}

int RM::howManyTasksCanBePerformed(const Task& task, const Resource& res)
{
    int count = 0;
    // отримуємо інформацію про вільні параметри ресурсу
    RM::ResourceRemaining resRem = RM::getResourceRemainingData(res);
    while (true)
    {
        // віднімаємо від параметрів ресурсу параметри, 
        // які потрібні для виконання однієї задачі
        resRem.discSize -= task.resDesc.discSize;
        resRem.memSize -= task.resDesc.memSize;
        resRem.procCount -= task.resDesc.procCount;

        // якщо хоча б один з параметрів менше 0, це означає, що
        // цього компоненту вже не вистачить для виконання задачі,
        // тому виходимо з циклу і повертаємо результат
        if (resRem.discSize < 0 || resRem.memSize < 0 || resRem.procCount < 0)
            break;
        // якщо ж всіх параметрів вистачило, збільшуємо результуючу кількість
        ++count;
    }
    return count;
}

double RM::getMinTimeToFree(const Resource& res)
{
    // якщо задач в ресурсі і так немає, повертаємо 0
    if (res.performingTasks.empty())
        return 0;
    // шукаємо найменше поле performTime з усіх завдань
    double minTimeToFree = res.performingTasks[0]->performTime;
    for (const Task* task : res.performingTasks)
    {
        if (task->performTime < minTimeToFree)
            minTimeToFree = task->performTime;
    }
    return minTimeToFree;
}

Resource* RM::findAnyFreeResource(
    const Task& task,
    const vector<Resource*>& resources,
    bool areSubTasksConnected
)
{
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

bool RM::canTaskBeSentToResource(
    const Task& task,
    const Resource& res,
    bool areSubTasksConnected,
    int* outSubTasksCount
)
{
    bool canTaskBeSent = false;
    int count = ResourceManager::howManyTasksCanBePerformed(task, res);

    // якщо задачі пов'язані, то ресурс треба звільнити так,
    // щоб завдання могло повністю виконатися за один раз
    if (areSubTasksConnected)
    {
        if (count >= task.count)
            canTaskBeSent = true;
    }
    // інакше достатньо можливості виконання будь-якої кількості
    // задач на ресурсі
    else
    {
        if (count > 0)
            canTaskBeSent = true;
    }
    // якщо count більше ніж загальна кількість задач, то
    // встановлюємо для count значення кількості задач
    count = (count > task.count) ? task.count : count;

    // перевірка на nullptr
    if (outSubTasksCount)
        *outSubTasksCount = count;

    return canTaskBeSent;
}

double RM::calcResourceStagnation(vector<Resource>& resources)
{
    double busyProcessors = 0;
    double allProcessors = 0;
    for (const Resource& res : resources)
    {
        allProcessors += res.resDesc.procCount;
        busyProcessors += RM::getResourceRemainingData(res).procCount;
    }
    if (allProcessors == 0)
        return 0;
    return busyProcessors / allProcessors * 100;
}

const Resource* RM::findResourceTaskIsPerfOn(
    const Task* task,
    const vector<Resource>& resources
)
{
    for (const Resource& res : resources)
    {
        for (const Task* taskOnRes : res.performingTasks)
        {
            if (task == taskOnRes)
                return &res;
        }
    }
    return nullptr;
}