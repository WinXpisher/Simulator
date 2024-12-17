#pragma once
#include <string>
#include <vector>

using namespace std;
// описувач ресурсу
struct ResourceDescriptor
{
    string procArch; // архітектура процесора
    string os; // операційна система
    int procCount; // кількість процесорів
    int procSpeed; // швидкодія процесорів
    int memSize; // об'єм оперативної пам'яті
    int discSize; // доступний обсяг вінчестера
};

struct Task;
struct Resource
{
    ResourceDescriptor resDesc; // опис ресурсу
    double bandwidth; // пропускна здатність каналу (від брокера до ресурсу)
    // задачі, які виконуються в даний момент часу
    // якщо вектор пустий, то таких задач немає
    vector<Task> performingTasks;
};

// завдання, яке включає пакет задач
struct Task
{
    enum TaskStatus
    {
        WAITING,
        CANCELLED
    };
    TaskStatus status; // статус завдання
    int count; // кількість задач в завданні
    ResourceDescriptor resDesc; // опис ресурсу, на якому кожна задача може виконатись
    double connectivity; // коефіцієнт зв'язності задач у завданні
    int priority; // пріоритет завдання
    double performTime; // час виконання задачі
    double subTaskSize; // розмір задачі (виконуючого файлу, який буде надсилатися)
};

struct DataBase
{
    vector<Task> tasks; // завдання, які треба виконати
    vector<Resource> availableResources; // доступні ресурси
};