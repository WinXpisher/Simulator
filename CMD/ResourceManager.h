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
    // Метод повертає доступні компоненти ресурсу, які ще можна зайняти.
    // Параметр considerStatus використовується в інтерфейсі користувача
    // і якщо встановлений як true, то враховуються тільки ті завдання,
    // в яких статус співпадає з параметром tStatus.
    static ResourceRemaining getResourceRemainingData(
        const Resource& res,
        bool considerStatus=false,
        Task::TaskStatus tStatus=Task::TaskStatus::RUNNING
    );
    // скільки задач певного завдання може бути виконано на даному ресурсі
    static int howManyTasksCanBePerformed(const Task& task, const Resource& res);
    // Чи можна відправити це завдання до ресурсу частково або повністю.
    // Якщо передати параметр outSubTasksCount і метод поверне true, то
    // в цю адресу буде записана кількість задач, які можуть бути виконані ресурсом.
    static bool canTaskBeSentToResource(
        const Task& task,
        const Resource& res,
        bool areSubTasksConnected,
        int* outSubTasksCount = nullptr
    );
    // отримати мінімальний час для звільнення ресурсу хоча б від однієї задачі
    static double getMinTimeToFree(const Resource& res);
    // Знайти будь-який ресурс, який може виконати дане завдання
    // частково або повністю (залежить від зв'язності завдань).
    // Якщо такого немає, метод повертає nullptr.
    static Resource* findAnyFreeResource(
        const Task& task,
        const vector<Resource*>& resources,
        bool areSubTasksConnected
    );
    // розрахувати простій ресурсів в відсотках
    static double calcResourceStagnation(vector<Resource>& resources);
    // знайти ресурс, на якому вже виконується завдання
    static const Resource* findResourceTaskIsPerfOn(
        const Task* task,
        const vector<Resource>& resources
    );
};