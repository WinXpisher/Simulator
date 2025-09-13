#pragma once
#include <string>
#include <vector>
#include <list>

#include <fstream>

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

    friend std::istream& operator>>(
        std::istream& is,
        ResourceDescriptor& resDesc
    );
    friend std::ostream& operator<<(
        std::ostream& os,
        const ResourceDescriptor& resDesc
    );
};

struct Task;
struct Resource
{
private:
    // у ресурсів буквенний айді
    static std::string globalId;
public:
    // генерація буквенного айді в наступному форматі
    // A, B, C, ..., Z, AA, AB, ..., ZZ, AAA, ...
    static std::string generateId();
    
    std::string id;
    ResourceDescriptor resDesc; // опис ресурсу
    double bandwidth; // пропускна здатність каналу (від брокера до ресурсу)
    // задачі, які виконуються в даний момент часу
    // якщо вектор пустий, то таких задач немає
    vector<Task*> performingTasks;

    friend std::istream& operator>>(
        std::istream& is,
        Resource& res
        );
    friend std::ostream& operator<<(
        std::ostream& os,
        const Resource& res
        );
};

// інформація завдань, пов'язана з симуляцією
struct SimulationInfo
{
    // покажчик на батьківське завдання (для не дочірніх завдань
    // встановлюється в nullptr)
    Task* parentTaskPtr;
    double waitingTime; // скільки часу завдання чекає
    double timePerformed; // скільки часу завдання вже виконується
    // Дочірні завдання (якщо задача має низький коефіцієнт зв'язності,
    // завдання можна розділити на частини, щоб розподілити на декілька
    // ресурсів). Використовуємо list, щоб гарантувати, що пам'ять не буде
    // перерозподілятися 
    std::list<Task> childTasks;
};

// завдання, яке включає пакет задач
struct Task
{
private:
    // у завдань числовий айді
    static int globalId;
public:
    // Метод підраховує скільки залишилося нерозподілених на 
    // ресурси задач. Вираховується різниця між кількістю задач
    // parentTask і сумарною кількістю задач дочірніх завдань.
    static int getRemainingSubTasksCount(const Task& parentTask);
    // генерація чисельного айді
    static std::string generateId();

    enum TaskStatus
    {
        WAITING,   // завдання очікує
        CANCELLED, // завдання скасовано
        SENDING,   // завдання надсилається на ресурс
        RUNNING,   // завдання запущено
        PERFORMED, // завдання виконано
        // завдання розділено на менші завдання (дочірні завдання),
        // але його частина все ще очікує
        DIVIDED,
        // завдання розділено на менші завдання (дочірні завдання)
        // і всі ці дочірні завдання або надсилаються або виконуються
        // (або обидва варіанти), але не очікують
        DIVIDED_RUNNING
        /* Якщо завдання має статус DIVIDED_RUNNING і всі його
            дочірні завдання мають статус PERFORMED, то таке завдання
            вважається виконаним і йому присвоюється статус PERFORMED.
        */
    };
    std::string id;
    TaskStatus status; // статус завдання
    int count; // кількість задач в завданні
    ResourceDescriptor resDesc; // опис ресурсу, на якому кожна задача може виконатись
    double connectivity; // коефіцієнт зв'язності задач у завданні
    int priority; // пріоритет завдання
    double performTime; // час виконання однієї задачі
    double subTaskSize; // розмір задачі (виконуючого файлу, який буде надсилатися)
    SimulationInfo simulationInfo; // додаткова інформація, пов'язана з виконанням

    friend std::istream& operator>>(
        std::istream& is,
        Task& task
        );
    friend std::ostream& operator<<(
        std::ostream& os,
        const Task& task
        );
};

struct DataBase
{
    vector<Task> tasks; // завдання, які треба виконати
    vector<Resource> availableResources; // доступні ресурси
};
