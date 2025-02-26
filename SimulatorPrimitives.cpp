#include "SimulatorPrimitives.h"

int Task::globalId = 1;
std::string Resource::globalId = "";

std::string Resource::generateId()
{
    if (globalId.empty()) 
    {
        globalId = "A";
        return globalId;
    }

    int i = globalId.size() - 1;
    // доки літера є останньою в алфавіті - Z,
    // замінюємо її на першу - A
    while (i >= 0 && globalId[i] == 'Z') 
    {
        globalId[i] = 'A';
        --i;
    }
    // якщо всі літери були Z, додаємо новий разряд
    if (i < 0)
        globalId.insert(globalId.begin(), 'A');
    // інакше, просто замінюємо останню (з кінця) літеру,
    // яка не була Z на наступну по алфавіту
    else
        globalId[i]++;

    return globalId;
}

std::string Task::generateId()
{
    return std::to_string(globalId++); 
}

int Task::getRemainingSubTasksCount(const Task& parentTask)
{
    int countToSubstract = 0;
    // підраховуємо суму задач всіх дочірніх завдань поточного завдання
    for (const Task& child : parentTask.simulationInfo.childTasks)
    {
        countToSubstract += child.count;
    }
    // повертаємо різницю задач батьківського завдання і підрахованої суми
    return parentTask.count - countToSubstract;
}