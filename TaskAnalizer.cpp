#include "TaskAnalizer.h"

void TaskAnalizer::analizeAllTasks()
{
    // викликаємо метод, щоб отримати результати для кожного завдання
    // причому результати будуть записані в поле conResult
    analizeConnectivity();
    for (ConnectivityResult& cr : conResult)
    {
        analizeTask(*cr.task, cr.areConnected);
    }
    // заповнюємо також вектор, де результат очищений від скасованих завдань
    clearFromCancelled();
}

void TaskAnalizer::clearFromCancelled()
{
    anResultClear.clear();
    for (auto& anRes : anResult)
    {
        // пропускаємо завдання
        if (anRes.task->status == Task::TaskStatus::CANCELLED)
            continue;
        anResultClear.push_back(anRes);
    }
}

void TaskAnalizer::analizeConnectivity()
{
    bool isConnected;
    for (Task& task : dataBase->tasks)
    {
        if (areSubTasksConnected(task))
            isConnected = true;
        else isConnected = false;
        // формуємо результуючий вектор
        conResult.push_back({ &task, isConnected });
    }
}

bool TaskAnalizer::areSubTasksConnected(const Task& task)
{
    // якщо коефіцієнт зв'язності більше 0.3, задачі вважаються зв'язаними
    return task.connectivity > 0.3;
}

void TaskAnalizer::analizeTask(Task& task, bool isConnected)
{
    bool wasFound = false; // чи був знайдений хоча б один ресурс
    AnalizerResult anRes;
    anRes.task = &task;
    for (Resource& res : dataBase->availableResources)
    {
        // якщо завдання не може бути виконано, пропускаємо ітерацію
        // в залежності чи є задачі пов'язані чи ні викликаються різні методи перевірки 
        if (isConnected && !canBePerformedConnected(*anRes.task, res))
            continue;
        if (!isConnected && !canBePerformedSimple(*anRes.task, res))
            continue;

        wasFound = true;
        // додаємо покажчик на ресурс, на якому завдання може бути виконане
        anRes.resources.push_back(&res);
    }
    // якщо для завдання існує хоч один ресурс, на якому воно може виконатися
    // встановлюємо статус WAITING
    if (wasFound)
        anRes.task->status = Task::TaskStatus::WAITING;
    // інакше встановлюємо статус CANCELLED
    else
        anRes.task->status = Task::TaskStatus::CANCELLED;
    // додаємо результат до загального вектору результатів
    anResult.push_back(anRes);
}

bool TaskAnalizer::canBePerformedSimple(const Task& task, Resource& res)
{
    // якщо процесорів не достатньо, завдання виконатися не може
    if (task.resDesc.procCount > res.resDesc.procCount)
        return false;

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

bool TaskAnalizer::canBePerformedConnected(const Task& task, Resource& res)
{
    // якщо процесорів не достатньо, завдання виконатися не може
    if ((task.count * task.resDesc.procCount) > res.resDesc.procCount)
        return false;
   
    // якщо оперативної пам'яті не достатньо, завдання виконатися не може
    if (task.count * task.resDesc.memSize > res.resDesc.memSize)
        return false;

    // якщо місця на диску не достатньо, завдання виконатися не може
    if (task.count * task.resDesc.discSize > res.resDesc.discSize)
        return false;

    return canBePerformedSimple(task, res);
}