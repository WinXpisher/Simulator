#include "DistributionMethod.h" 
#include "ResourceManager.h"


using DM = DistributionMethod;
const DM::DMethod DM::dMethods[8] = { LIFO, FCFS, HPF, BACKFILL, SIMPLEX, SMART, MFQS, Penguin, };

bool LIFO::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const
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
    TaskAnalizer::AnalizerResult& outAnResult
) const
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
    TaskAnalizer::AnalizerResult& outAnResult
) const
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


bool BACKFILL::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const
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
    TaskAnalizer::AnalizerResult& outAnResult
) const
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
    TaskAnalizer::AnalizerResult& outAnResult
) const
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

    // Видаляємо обране завдання зі списку
    tasks.erase(remove_if(tasks.begin(), tasks.end(),
        [&](const TaskAnalizer::AnalizerResult& task) {
            return task.task->id == selectedTask->task->id;
        }),
        tasks.end());
    return true;
}


bool MFQS::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult
) const {
    if (tasks.empty()) return false; // Перевіряємо, чи є завдання

    // Припустимо, що завдання розподілені за рівнями черг (приклад з 3 рівнями)
    vector<vector<TaskAnalizer::AnalizerResult>> queues(3); // 3 рівня черг

    // Розподіл завдань по чергам за їх пріоритетом
    for (auto& task : tasks) {
        if (task.task->priority >= 10) {
            queues[0].push_back(task); // Високий пріоритет
        }
        else if (task.task->priority >= 5) {
            queues[1].push_back(task); // Середній пріоритет
        }
        else {
            queues[2].push_back(task); // Низький пріоритет
        }
    }

    // Проходимо по чергам від найвищого пріоритету до найнижчого
    for (auto& queue : queues) {
        if (!queue.empty()) {
            // Отримуємо завдання з черги
            outAnResult = queue.front();
            queue.erase(queue.begin()); // Видаляємо його з черги

            // Видаляємо також завдання з початкового вектора
            tasks.erase(remove_if(tasks.begin(), tasks.end(), [&](const TaskAnalizer::AnalizerResult& task) {
                return task.task->id == outAnResult.task->id;
                }), tasks.end());
            return true;
        }
    }

    return false; // Якщо завдань у чергах немає
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
        // Оновлюємо найкращий ресурс, якщо оцінка вища
        if (score > bestScore)
        {
            bestScore = score;
            bestResource = resource;
        }
    }

    return bestResource;
}


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
    TaskAnalizer::AnalizerResult& outAnResult
) const
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

    // Видаляємо обране завдання зі списку
    tasks.erase(remove_if(tasks.begin(), tasks.end(),
        [&](const TaskAnalizer::AnalizerResult& task) {
            return task.task->id == selectedTask->task->id;
        }),
        tasks.end());
    return true;
}