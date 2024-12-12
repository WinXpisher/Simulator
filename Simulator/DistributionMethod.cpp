#include "DistributionMethod.h"

using DM = DistributionMethod;
const DM::DMethod DM::dMethods[1] = { LIFO };

bool LIFO::nextTask(vector<Task>& tasks, Task& outTask) const
{
    // якщо завдань не залишилось, повертаємо false
    if (tasks.empty())
        return false;

    // записуємо останнє завдання в зовнішню змінну
    // і видаляємо його з вектору
    outTask = tasks.back();
    tasks.pop_back();
    return true;
}