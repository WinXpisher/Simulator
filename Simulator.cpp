#include <iostream>
#include <vector>
#include "SimulatorPrimitives.h"
#include "SimulationEnvironment.h"
#include "DistributionMethod.h"
#include "ResourceManager.h"
using namespace std;

void printDMethod(DistributionMethod::DMethod dm)
{
    using DM = DistributionMethod;
    switch (dm)
    {
    case DM::FCFS:
        cout << "Method: FCFS\n";
        break;
    case DM::LIFO:
        cout << "Method: LIFO\n";
        break;
    case DM::HPF:
        cout << "Method: HPF\n";
        break;
    case DM::BACKFILL:
        break;
    case DM::SIMPLEX:
        cout << "Method: SIMPLEX\n";
        break;
    case DM::SMART:
        break;
    default:
        break;
    }
}


int main()
{
    DataBase db;
    
    // задачі
    Task t1 = {
        Task::WAITING,
        30, // кількість задач
        {"x86", "Linux", 15, 100, 8, 30},
        0.9,
        10,
        500, // час виконання однієї задачі
        20
    };
    Task t2 = {
        Task::WAITING,
        50, // кількість задач
        {"ARM", "Windows", 25, 20, 16, 100},
        0.2,
        20,
        300, // час виконання однієї задачі
        950
    };
    Task t3 = {
        Task::WAITING,
        3, // кількість задач
        {"x86", "Linux", 1, 1, 16, 500},
        0.2,
        3,
        95, // час виконання однієї задачі
        200
    };
    Task t4 = {
        Task::WAITING,
        4, // кількість задач
        {"ARM", "Windows 10", 1, -1, 100, 100},
        0.5,
        2,
        70, // час виконання однієї задачі
        150
    };
    Task t5 = {
        Task::WAITING,
        4, // кількість задач
        {"ARM", "Windows 10", 4, 14, 30, 50},
        0.4,
        2,
        300, // час виконання однієї задачі
        20
    };
    Task t6 = {
        Task::WAITING,
        33, // кількість задач
        {"x86", "Linux", 1, 25, 16, 500},
        0.2,
        19,
        450, // час виконання однієї задачі
        70
    };
    Task t7 = {
        Task::WAITING,
        2, // кількість задач
        {"x86", "Linux", 2, 31, 8, 100},
        0.1,
        40,
        400, // час виконання однієї задачі
        10
    };
    Task t8 = {
        Task::WAITING,
        20, // кількість задач
        {"x86", "Linux", 40, 200, 8, 700},
        0.5,
        40,
        50, // час виконання однієї задачі
        700
    };
    Task t9 = {
        Task::WAITING,
        100, // кількість задач
        {"x86", "Linux", 50, 31, 8, 100},
        0.3,
        40,
        100, // час виконання однієї задачі
        300
    };

    // ресурси
    Resource r1 = {
        {"ARM", "Windows", 100, 100, 16, 500},
        100,
    };
    Resource r2 = {
        {"ARM", "Windows", 100, 300, 800, 1000},
        200,
    };
    Resource r3 = {
        {"x86", "Linux", 30, 70, 88, 500},
        100,
    };
    Resource r4 = {
        {"ARM", "Windows 10", 50, 50, 20, 256},
        10,
    };
    Resource r5 = {
        {"x86", "Linux", 200, 300, 256, 2000},
        100,
    };
    Resource r6 = {
        {"ARM", "Windows 10", 150, 200, 256, 1028},
        1000,
    };

    // додаємо завдання
    db.tasks.push_back(t5);
    db.tasks.push_back(t6);
    db.tasks.push_back(t1);
    db.tasks.push_back(t7);
    db.tasks.push_back(t4);
    db.tasks.push_back(t2);
    db.tasks.push_back(t9);
    db.tasks.push_back(t4);
    db.tasks.push_back(t3);
    db.tasks.push_back(t5);
    db.tasks.push_back(t6);
    db.tasks.push_back(t2);
    db.tasks.push_back(t2);
    db.tasks.push_back(t8);

    // додаємо ресурси
    db.availableResources.push_back(r1);
    db.availableResources.push_back(r1);
    db.availableResources.push_back(r6);
    db.availableResources.push_back(r3);
    db.availableResources.push_back(r4);
    db.availableResources.push_back(r1);
    db.availableResources.push_back(r2);
    db.availableResources.push_back(r3);
    db.availableResources.push_back(r4);
    db.availableResources.push_back(r1);
    db.availableResources.push_back(r2);
    db.availableResources.push_back(r5);
    db.availableResources.push_back(r6);
    db.availableResources.push_back(r3);
    db.availableResources.push_back(r4);
    db.availableResources.push_back(r1);

    SimulationEnvironment simEnv(&db);
    simEnv.runSimulation();
    for (auto& simResult : simEnv.getResults())
    {
        printDMethod(simResult.methodId);
        cout << "exec: " << simResult.execTime << endl;
        cout << "avarage wait time: " << simResult.avWaitTime << endl;
    }

    return 0;
}