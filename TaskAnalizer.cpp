#include "TaskAnalizer.h"

void TaskAnalizer::analizeAllTasks()
{
    // викликаємо метод, щоб отримати результати для кожного завдання
    // причому результати будуть записані в поле conResult
    analizeConnectivity();
    for (ConnectivityResult& cr : conResult)
    {
        analizeTask(*cr.taskPtr, cr.areConnected);
    }
}

void TaskAnalizer::clearFromCancelled()
{
    anResultClear.clear();
    for (auto& anRes : anResult)
    {
        // пропускаємо завдання
        if (anRes.taskPtr->status == Task::TaskStatus::CANCELLED)
            continue;
        anResultClear.push_back(anRes);
    }
}

void TaskAnalizer::analizeConnectivity()
{
    bool isConnected;
    for (Task& task : dataBase->tasks)
    {
        // якщо коефіцієнт зв'язності більше 0.3, задачі вважаються зв'язаними
        if (task.connectivity > 0.3)
            isConnected = true;
        else isConnected = false;
        // формуємо результуючий вектор
        conResult.push_back({ &task, isConnected });
    }
}

void TaskAnalizer::analizeTask(Task& task, bool isConnected)
{
    bool wasFound = false; // чи був знайдений хоча б один ресурс
    AnalizerResult anRes;
    anRes.taskPtr = &task;
    for (Resource& res : dataBase->availableResources)
    {
        // якщо завдання не може бути виконано, пропускаємо ітерацію
        // в залежності чи є задачі пов'язані чи ні викликаються різні методи перевірки 
        if (isConnected && !canBePerformedConnected(task, res))
            continue;
        if (!isConnected && !canBePerformedSimple(task, res))
            continue;

        wasFound = true;
        // додаємо покажчик на ресурс, на якому завдання може бути виконане
        anRes.resources.push_back(&res);
    }
    // якщо для завдання існує хоч один ресурс, на якому воно може виконатися
    // встановлюємо статус WAITING
    if (wasFound)
        task.status = Task::TaskStatus::WAITING;
    // інакше встановлюємо статус CANCELLED
    else
        task.status = Task::TaskStatus::CANCELLED;
    // додаємо результат до загального вектору результатів
    anResult.push_back(anRes);
}

bool TaskAnalizer::canBePerformedSimple(Task& task, Resource& res)
{
    // перевіряємо сумісність архітектур процесора
    if (task.resDesc.procArch != res.resDesc.procArch)
        return false;

    // перевіряємо сумісність операційних систем
    if (task.resDesc.os != res.resDesc.os)
        return false;

    // якщо швидкість процесора не достатня, завдання виконатися не може
    if (task.resDesc.procSpeed > res.resDesc.procSpeed)
        return false;

    // якщо оперативної пам'яті не достатньо, завдання виконатися не може
    if (task.resDesc.memSize > res.resDesc.memSize)
        return false;

    // якщо місця на диску не достатньо, завдання виконатися не може
    if (task.resDesc.discSize > res.resDesc.discSize)
        return false;

    // якщо всі перевірки пройшли успішно, завдання може бути виконано
    return true;
}

bool TaskAnalizer::canBePerformedConnected(Task& task, Resource& res)
{
    // якщо процесорів не достатньо, завдання виконатися не може
    if (task.resDesc.procCount > res.resDesc.procCount)
        return false;

    return canBePerformedSimple(task, res);
}