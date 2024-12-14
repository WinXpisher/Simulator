#pragma once
#include "SimulatorPrimitives.h"

class ResourceManager
{
public:
    // задачі, які виконуються в даний момент займають якусь
    // кількість процесорів, пам'яті і тд
    // структура відображає вільні компоненти ресурсу
    struct ResourceRemaining
    {
        int procCount; // кількість процесорів
        int memSize; // об'єм оперативної пам'яті
        int discSize; // доступний обсяг вінчестера
    };
    // метод повертає доступні компоненти ресурсу, які ще можна зайняти
    static ResourceRemaining getResourceRemainingData(const Resource& res);
    // скільки задач певного завдання може бути виконано на даному ресурсі
    static int howManyTasksCanBePerformed(const Task& task, const Resource& res);
};