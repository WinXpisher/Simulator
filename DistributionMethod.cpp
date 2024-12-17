#include "DistributionMethod.h" 

using DM = DistributionMethod;
const DM::DMethod DM::dMethods[5] = { LIFO, FCFS, HPF, BACKFILL, SIMPLEX };

bool LIFO::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource* outRes) const
{
    // якщо завдань не залишилось, повертаємо false 
    if (tasks.empty())
        return false;

    // записуємо останнє завдання в зовнішню змінну 
    // і видаляємо його з вектору 
    outAnResult = tasks.back();
    tasks.pop_back();
    return true;
}

bool FCFS::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource* outRes) const
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

bool HPF::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource* outRes) const
{
    // Якщо завдань немає, повертаємо false 
    if (tasks.empty())
        return false;

    // Знаходимо завдання з найвищим пріорітетом  
    auto highestPriorityTaskIter = max_element(tasks.begin(), tasks.end(),
        [](const TaskAnalizer::AnalizerResult& a, const TaskAnalizer::AnalizerResult& b) {
            return a.task.priority < b.task.priority;
        });
    // Записужмо завдання з найвищим пріорітетом 
    TaskAnalizer::AnalizerResult highestPriorityTask = *highestPriorityTaskIter;
    outAnResult = highestPriorityTask;
    // Видаляємо оброблене завдання з вектору 
    tasks.erase(highestPriorityTaskIter);
    return true;
}

//bool BACKFILL::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
//    TaskAnalizer::AnalizerResult& outAnResult,
//    Resource* outRes) const
//{
//    // Якщо немає завдань повертаємо false 
//    if (tasks.empty())
//        return false;
//
//    TaskAnalizer::AnalizerResult selectedTask;
//    bool found = false;
//
//    for (auto it = tasks.begin(); it != tasks.end(); ++it)
//    {
//        // тут треба работа з ресурсами, щоб проаналізувати яке завдання є найпростішим відносно ресурсів, які є 
//    }
//
//    // Якщо немає завдання відповідно до ресурсів 
//    if (!found)
//    {
//        return false;
//    }
//    outAnResult = selectedTask;
//    // Прибираємо обране завдання зі списку завдань 
//    tasks.erase(remove(tasks.begin(), tasks.end(), selectedTask), tasks.end());
//    return true;
//}

bool SIMPLEX::nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
    TaskAnalizer::AnalizerResult& outAnResult,
    Resource* outRes) const
{
    // Якщо немає завдань поертаємо false 
    if (tasks.empty())
        return false;

    // Пошук завдання за певним критерієм, наприклад, вибір мінімального часу виконання 
    size_t selectedIdx = 0;
    for (size_t i = 1; i < tasks.size(); ++i)
    {
        // Вибір завдання за певним критерієм (наприклад, по мінімальному часу виконання або якщо багато вільних ресурсів) 
        if (tasks[i].task.performTime < tasks[selectedIdx].task.performTime)
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