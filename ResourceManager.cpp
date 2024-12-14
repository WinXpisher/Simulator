#include "ResourceManager.h"
using RM = ResourceManager;

RM::ResourceRemaining RM::getResourceRemainingData(
    const Resource& res)
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
        // множимо кожен параметр на кількість задач із
        // завдання, які виконуються в даний момент
        resRem.discSize -= task.resDesc.discSize * task.count;
        resRem.memSize -= task.resDesc.memSize * task.count;
        resRem.procCount -= task.resDesc.procCount * task.count;
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