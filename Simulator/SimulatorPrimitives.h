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

struct Resource
{
    ResourceDescriptor resDesc; // опис ресурсу
    double bandwidth; // сумарна пропускна здатність каналу (від брокера до ресурсу)
    double delay; // сумарна затримка часу передачі пакета
    // час, через який ресурс буде звільнено, якщо releaseTime=0,
    // то ресурс вже вільний
    double releaseTime;
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
    ResourceDescriptor resDesc; // опис ресурсу, на якому завдання може виконатись
    double connectivity; // коефіцієнт зв'язності задач у завданні
    int priority; // пріоритет завдання
    double performTime; // час виконання завдання
};

struct DataBase
{
    vector<Task> tasks; // завдання, які треба виконати
    vector<Resource> availableResources; // доступні ресурси
};