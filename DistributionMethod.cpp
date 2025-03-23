#include "DistributionMethod.h" 
#include "ResourceManager.h"

using DM = DistributionMethod;
const DM::DMethod DM::dMethods[8] = { LIFO, FCFS, HPF, BACKFILL, SIMPLEX, SMART, MFQS, Penguin, };

bool LIFO::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    if (tasks.empty()) // Перевіряємо, чи є завдання
        return false;

    // Отримуємо останнє завдання
    outAnResult = tasks.back();

    // Видаляємо його зі списку
    tasks.pop_back();
    return true;
}

bool FCFS::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    // Якщо завдань немає, повертаємо false 
    if (tasks.empty())
        return false;
    // Отримаємо перше завдання в черзі 
    TaskAnalizer::AnalizerResult& taskResult = tasks.front();
    // Записуємо вихідне завдання з результату 
    outAnResult = taskResult;
    // Видаляємо оброблене завдання з вектору 
    tasks.erase(tasks.begin());
    return true;
}

bool HPF::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    if (tasks.empty())
        return false;

    // Знаходимо завдання з найвищим пріоритетом
    auto highestPriorityTaskIter = max_element(
        tasks.begin(), tasks.end(),
        [](const TaskAnalizer::AnalizerResult& a, const TaskAnalizer::AnalizerResult& b) {
            return a.task->priority < b.task->priority;
        });

    // Безпосередньо працюємо з ітератором, щоб уникнути копіювання
    outAnResult = *highestPriorityTaskIter;
    tasks.erase(highestPriorityTaskIter);
    return true;
}

//bool BACKFILL::nextTask(
//    vector<TaskAnalizer::AnalizerResult>& tasks,
//    TaskAnalizer::AnalizerResult& outAnResult,
//    Resource*& outRes) const
//{
//    if (tasks.empty())
//        return false;
//
//    // Ітерація по задачах для знаходження тієї, яка може бути виконана з доступними ресурсами
//    for (auto it = tasks.begin(); it != tasks.end(); ++it)
//    {
//        for (auto& res : it->resources)
//        {
//            // Перевіряємо, чи ресурс відповідає вимогам завдання
//            if (res->resDesc.procCount >= it->task->resDesc.procCount &&
//                res->resDesc.memSize >= it->task->resDesc.memSize &&
//                res->resDesc.discSize >= it->task->resDesc.discSize)
//            {
//                // Обираємо поточне завдання
//                outAnResult = *it;
//                outRes = res;
//                tasks.erase(it);
//                return true;
//            }
//        }
//    }
//
//    // Якщо не знайдено відповідного завдання
//    return false;
//}



double calculateScore(const Task* task, const Resource* resource)
{
    static const double PRIORITY_WEIGHT = 10.0;
    static const double PERFORM_TIME_WEIGHT = -1.0;
    static const double CONNECTIVITY_WEIGHT = 5.0;
    static const double BANDWIDTH_WEIGHT = 2.0;

    return task->priority * PRIORITY_WEIGHT +
        task->performTime * PERFORM_TIME_WEIGHT +
        task->connectivity * CONNECTIVITY_WEIGHT +
        resource->bandwidth * BANDWIDTH_WEIGHT;
}

Resource* findOptimizedResource(
    const Task& task,
    const vector<Resource*>& freeResources,
    bool areSubTasksConnected)
{
    if (freeResources.empty())
        return nullptr;

    Resource* bestResource = nullptr;
    double bestScore = std::numeric_limits<double>::lowest();
    for (auto& resource : freeResources)
    {
        // Отримуємо залишкові компоненти ресурсу
        auto remaining = ResourceManager::getResourceRemainingData(*resource);

        // Розраховуємо оцінку ресурсу
        double score = 0.0;
        score += remaining.procCount * 60; // Більше процесорів - краще
        score += remaining.memSize * 3;   // Більше пам'яті - краще
        score += remaining.discSize;  // Більше дискового простору - краще
        score += resource->bandwidth * 4; // Пропускна здатність
        
        score -= resource->resDesc.procSpeed * 20;
        //// Якщо завдання пов'язане, враховуємо кількість процесорів
        //if (areSubTasksConnected)
        //    score -= abs(resource->resDesc.procCount - task.resDesc.procCount);

        // Оновлюємо найкращий ресурс, якщо оцінка вища
        if (score > bestScore)
        {
            bestScore = score;
            bestResource = resource;
        }
    }

    return bestResource;
}

Resource* BACKFILL::nextResource(
    TaskAnalizer::AnalizerResult& anResult
) const
{
    //return nullptr;
    DataBase db; // Ініціалізація бази даних
    TaskAnalizer analyzer(&db);
    vector<Resource*> freeResources;
    for (auto& res : anResult.resources)
    {
        // Перевіряємо, чи може завдання виконатися на ресурсі в поточний момент
        if (ResourceManager::canTaskBeSentToResource(
            *anResult.task,
            *res,
            analyzer.areSubTasksConnected(*anResult.task)))
        {
            freeResources.push_back(res);
        }
    }

    return findOptimizedResource(
        *anResult.task,
        freeResources,
        analyzer.areSubTasksConnected(*anResult.task)
    );
}

bool BACKFILL::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    if (tasks.empty())
        return false;

    DataBase db; // Ініціалізація бази даних
    TaskAnalizer analyzer(&db);
    Resource* resToSelect = nullptr;
    // Ітерація по задачах для знаходження тієї, яка може бути виконана з доступними ресурсами
    for (auto it = tasks.begin(); it != tasks.end(); ++it)
    {
        // Перевіряємо, чи може завдання виконатися на ресурсі в поточний момент
        if (resToSelect = ResourceManager::findAnyFreeResource(
            *(*it).task,
            (*it).resources,
            analyzer.areSubTasksConnected(*(*it).task)))
        {
            // Обираємо поточне завдання
            outAnResult = *it;
            tasks.erase(it);
            return true;
        }
    }
    // Отримуємо останнє завдання
    outAnResult = tasks.back();

    // Видаляємо його зі списку
    tasks.pop_back();
    return true;
}

bool SIMPLEX::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    // Якщо немає завдань поертаємо false
    if (tasks.empty())
        return false;

    // Пошук завдання за певним критерієм,
    size_t selectedIdx = 0;
    for (size_t i = 1; i < tasks.size(); ++i)
    {
        // Вибір завдання за певним критерієм
        if (tasks[i].task->count > tasks[selectedIdx].task->count ||
            tasks[i].task->connectivity > tasks[selectedIdx].task->connectivity ||
            tasks[i].task->subTaskSize > tasks[selectedIdx].task->subTaskSize)
        {
            selectedIdx = i;
        }
    }

    // Призначення знайденого завдання
    outAnResult = tasks[selectedIdx];

    // Видалити вибране завдання з списку
    tasks.erase(tasks.begin() + selectedIdx);

    return true;
}

bool SMART::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    if (tasks.empty())
        return false;

    TaskAnalizer::AnalizerResult* selectedTask = &tasks[0];
    Resource* selectedResource = tasks[0].resources[0];
    double bestScore = std::numeric_limits<double>::min();;

    // Проходимо по всіх завданнях і ресурсах для пошуку оптимального варіанту
    for (auto& taskResult : tasks)
    {
        for (auto& resource : taskResult.resources)
        {
            // Обчислюємо оцінку завдання за допомогою декількох критеріїв
            double score = 0.0;
            score += taskResult.task->priority * 10;  // Пріоритет завдання
            score -= taskResult.task->performTime;    // Час виконання завдання (менший час - краще)
            score += taskResult.task->connectivity * 5;  // Коефіцієнт зв'язності
            score += resource->bandwidth * 2;           // Пропускна здатність ресурсу

            // Якщо завдання має кращу оцінку, вибираємо його
            if (score > bestScore)
            {
                bestScore = score;
                selectedTask = &taskResult;
                selectedResource = resource;
            }
        }
    }

    // Призначаємо знайдене завдання і ресурс
    outAnResult = *selectedTask;
    //outRes = selectedResource;

    // Видаляємо обране завдання зі списку
    tasks.erase(remove_if(tasks.begin(), tasks.end(),
        [&](const TaskAnalizer::AnalizerResult& task) {
            return task.task->id == selectedTask->task->id;
        }),
        tasks.end());
    return true;
}



//bool MFQS::nextTask(
//    vector<TaskAnalizer::AnalizerResult>& tasks,
//    TaskAnalizer::AnalizerResult& outAnResult,
//    Resource*& outRes) const
//{
//    // Додаємо всі завдання до найвищої черги, якщо вони ще не розподілені
//    for (auto& taskResult : tasks)
//    {
//        addToQueue(taskResult, 0); // Початкове завдання додається в першу чергу
//    }
//
//    // Проходимо по чергах (з найвищого до найнижчого рівня)
//    for (size_t i = 0; i < priorityQueues.size(); ++i)
//    {
//        auto& queue = priorityQueues[i];
//        while (!queue.empty())
//        {
//            auto task = queue.front(); // Беремо перше завдання з черги
//            queue.pop_front();         // Видаляємо його з черги
//
//            // Перевіряємо, чи підходить ресурс для виконання завдання
//            for (auto& resource : task.resources)
//            {
//                if (resource->resDesc.procCount >= task.task->resDesc.procCount &&
//                    resource->resDesc.memSize >= task.task->resDesc.memSize &&
//                    resource->resDesc.discSize >= task.task->resDesc.discSize)
//                {
//                    // Призначаємо завдання і ресурс
//                    outAnResult = task;
//                    outRes = resource;
//                    return true;
//                }
//            }
//
//            // Якщо ресурс не знайдено, завдання переходить у нижчу чергу
//            if (i + 1 < priorityQueues.size())
//            {
//                addToQueue(task, i + 1);
//            }
//        }
//    }
//
//    // Отримуємо останнє завдання
//    outAnResult = tasks.back();
//
//    // Видаляємо його зі списку
//    tasks.pop_back();
//}

//bool MFQS::nextTask(
//    vector<TaskAnalizer::AnalizerResult>& tasks,
//    TaskAnalizer::AnalizerResult& outAnResult,
//    Resource*& outRes) const
//{
//    if (tasks.empty()) // Якщо завдань немає
//        return false;
//
//    DataBase db; // Ініціалізація бази даних
//    TaskAnalizer analyzer(&db); // Коректне створення об'єкта `TaskAnalizer`
//
//    // Проходимо по всіх рівнях черг
//    for (size_t i = 0; i < priorityQueues.size(); ++i)
//    {
//        auto& queue = priorityQueues[i];
//
//        while (!queue.empty())
//        {
//            auto task = queue.front();
//            queue.pop_front();
//
//            bool areConnected = analyzer.areSubTasksConnected(*task.task);
//
//            Resource* suitableResource = ResourceManager::findAnyFreeResource(
//                *task.task, task.resources, areConnected
//            );
//
//            if (suitableResource)
//            {
//                outAnResult = task;
//                outRes = suitableResource;
//                return true;
//            }
//
//            if (i + 1 < priorityQueues.size())
//                addToQueue(task, i + 1);
//        }
//    }
//
//    // Отримуємо останнє завдання
//    outAnResult = tasks.back();
//
//    // Видаляємо його зі списку
//    tasks.pop_back();
//}

//bool Penguin::nextTask(
//    vector<TaskAnalizer::AnalizerResult>& tasks,
//    TaskAnalizer::AnalizerResult& outAnResult,
//    Resource*& outRes) const
//{
//    if (tasks.empty()) // Перевіряємо, чи є завдання
//        return false;
//
//    TaskAnalizer::AnalizerResult* selectedTask = nullptr;
//    Resource* selectedResource = nullptr;
//    double minWeight = std::numeric_limits<double>::max(); // Для пошуку мінімального значення
//
//    // Проходимо по всіх завданнях і ресурсах
//    for (auto& taskResult : tasks)
//    {
//        for (auto& resource : taskResult.resources)
//        {
//            // Розрахунок "ваги" ресурсів завдання
//            double weight = taskResult.task->resDesc.procCount +
//                taskResult.task->resDesc.memSize +
//                taskResult.task->resDesc.discSize;
//
//            // Якщо завдання має меншу вагу, ніж попереднє найкраще, вибираємо його
//            if (weight < minWeight)
//            {
//                minWeight = weight;
//                selectedTask = &taskResult;
//                selectedResource = resource;
//            }
//        }
//    }
//
//    // Якщо не знайдено підходящого завдання
//    if (selectedTask == nullptr)
//        return false;
//
//    // Призначаємо знайдене завдання і ресурс
//    outAnResult = *selectedTask;
//    //outRes = selectedResource;
//
//    // Видаляємо обране завдання зі списку
//    tasks.erase(remove_if(tasks.begin(), tasks.end(),
//        [&](const TaskAnalizer::AnalizerResult& task) {
//            return task.task->id == selectedTask->task->id;
//        }),
//        tasks.end());
//    return true;
//}


Resource* Penguin::nextResource(
    TaskAnalizer::AnalizerResult& anResult
) const
{
    DataBase db; // Ініціалізація бази даних
    TaskAnalizer analyzer(&db);
    vector<Resource*> freeResources;
    for (auto& res : anResult.resources)
    {
        // Перевіряємо, чи може завдання виконатися на ресурсі в поточний момент
        if (ResourceManager::canTaskBeSentToResource(
            *anResult.task,
            *res,
            analyzer.areSubTasksConnected(*anResult.task)))
        {
            freeResources.push_back(res);
        }
    }

    return findOptimizedResource(
        *anResult.task,
        freeResources,
        analyzer.areSubTasksConnected(*anResult.task)
    );
}


bool Penguin::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource*& outRes) const
{
    if (tasks.empty()) // Перевіряємо, чи є завдання
        return false;

    TaskAnalizer::AnalizerResult* selectedTask = nullptr;
    Resource* selectedResource = nullptr;
    double minWeight = std::numeric_limits<double>::max(); // Для пошуку мінімального значення

    // Проходимо по всіх завданнях і ресурсах
    for (auto& taskResult : tasks)
    {
        for (auto& resource : taskResult.resources)
        {
            // Розрахунок "ваги" ресурсів завдання
            double weight = 6 * taskResult.task->count *
                taskResult.task->resDesc.procCount +
                8 * taskResult.task->resDesc.memSize +
                taskResult.task->resDesc.discSize;

            // Якщо завдання має меншу вагу, ніж попереднє найкраще, вибираємо його
            if (weight < minWeight)
            {
                minWeight = weight;
                selectedTask = &taskResult;
                selectedResource = resource;
            }
        }
    }

    // Якщо не знайдено підходящого завдання
    if (selectedTask == nullptr)
        return false;

    // Призначаємо знайдене завдання і ресурс
    outAnResult = *selectedTask;
    //outRes = selectedResource;

    // Видаляємо обране завдання зі списку
    tasks.erase(remove_if(tasks.begin(), tasks.end(),
        [&](const TaskAnalizer::AnalizerResult& task) {
            return task.task->id == selectedTask->task->id;
        }),
        tasks.end());
    return true;
}