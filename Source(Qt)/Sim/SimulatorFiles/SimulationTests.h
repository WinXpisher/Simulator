#pragma once
#include "SimulatorPrimitives.h"

// TEST 1 - always busy resource, resources' stagnation - 0%.
// Also, that shows how tasks with a low coefficient of connectivity
// distributes on the small tasks
void fillTasks1(DataBase& db)
{
    Task t1 = {
        Task::generateId(),
        Task::WAITING,
        10, // the number of tasks
        {
            "x86",
            "Windows 10",
            1,  // the number of processors
            10, // processors speed
            8,  // RAM size
            512 // available size of HardDrive
        },
        0.2,  // tasks' connectivity coefficient
        5,    // priority of task
        6,    // finished time for 1 task
        20    // size of task
    };
    db.tasks.push_back(t1);
}

void fillResources1(DataBase& db)
{
    Resource r1 = {
        Resource::generateId(),
        {
            "x86",
            "Windows 10",
            2,  // the number of processors
            10, // processors speed
            64,  // RAM size
            3000 // available size of HardDrive
        },
        10, // channel's bandwith
    };
    db.availableResources.push_back(r1);
}

// TEST 2 - avarage waiting time is 0.
// Tasks are being sent immediately.
// Also, half of resources will be stagnated,
// for this reason, the resources' stagnation will be 50%.
// !!! Description for the test is correct,
// when the number of connectivity channels equals 2.
void fillTasks2(DataBase& db)
{
    Task t = {
        Task::generateId(),
        Task::WAITING,
        1, // the number of tasks
        {
            "x86",
            "Windows 10",
            1,  // the number of processors
            10, // processors' speed
            8,  // RAM size
            512 // available size of HardDrive
        },
        0.2,  // tasks' connectivity coefficient
        5,    // priority of task
        25,   // finished time for 1 task
        50    // size of task
    };
    // Adding 2 similar tasks with different ids
    db.tasks.push_back(t);
    t.id = Task::generateId();
    db.tasks.push_back(t);
}

void fillResources2(DataBase& db)
{
    Resource r = {
        Resource::generateId(),
        {
            "x86",
            "Windows 10",
            1,  // the number of processors
            10, // processors' speed
            64,  // RAM size
            3000 // available size of HardDrive
        },
        10, // channel's bandwith
    };
    //  Adding 4 similar resources with different ids
    db.availableResources.push_back(r);
    r.id = Resource::generateId();
    db.availableResources.push_back(r);
    r.id = Resource::generateId();
    db.availableResources.push_back(r);
    r.id = Resource::generateId();
    db.availableResources.push_back(r);
}

// TEST 3 - many tasks and resources.
// Good demonstration of the sending and distribution method.
// Also, one of the tasks is declined, because
// none of exist of neededresources is available
void fillTasks3(DataBase& db)
{
    Task t1 = {
        Task::generateId(),
        Task::WAITING,
        6, // the number of tasks
        {
            "x86",
            "Windows 10",
            1,  // the number of processors
            10, // processors' speed
            8,  // RAM size
            512 // available size of HardDrive
        },
        0.2,  // tasks' connectivity coefficient
        5,    // priority of task
        6,    // finished time for 1 task
        20    // size of task
    };
    db.tasks.push_back(t1);

    Task t2 = {
        Task::generateId(),
        Task::WAITING,
        3, // the number of tasks
        {
            "x86",
            "Windows 10",
            1,  // the number of processors
            10, // processors' speed
            8,  // RAM size
            512 // available size of HardDrive
        },
        0.4,  // tasks' connectivity coefficient
        1,    // priority of task
        6,    // finished time for 1 task
        10    // size of task
    };
    db.tasks.push_back(t2);

    Task t3 = {
        Task::generateId(),
        Task::WAITING,
        6, // the number of tasks
        {
            "x86",
            "Windows 10",
            1,  // the number of processors
            10, // processors' speed
            8,  // RAM size
            512 // available size of HardDrive
        },
        0.5,  // tasks' connectivity coefficient
        3,    // priority of task
        6,    // finished time for 1 task
        10    // size of task
    };
    db.tasks.push_back(t3);

    Task t4 = {
        Task::generateId(),
        Task::WAITING,
        5, // the number of tasks
        {
            "x86",
            "Windows 11",
            1,  // the number of processors
            10, // processors' speed
            8,  // RAM size
            512 // available size of HardDrive
        },
        0.6,  // tasks' connectivity coefficient
        7,    // priority of task
        6,    // finished time for 1 task
        20    // size of task
    };
    db.tasks.push_back(t4);

    Task t5 = {
        Task::generateId(),
        Task::WAITING,
        5, // the number of tasks
        {
            "x86",
            "Windows 11",
            1,  // the number of processors
            10, // processors' speed
            8,  // RAM size
            512 // available size of HardDrive
        },
        0.6,  // tasks' connectivity coefficient
        4,    // priority of task
        36,    // finished time for 1 task
        20    // size of task
    };
    db.tasks.push_back(t5);

    Task t6 = {
        Task::generateId(),
        Task::WAITING,
        5, // the number of tasks
        {
            "x86",
            "Windows 11",
            1,  // the number of processors
            10, // processors' speed
            8,  // RAM size
            512 // available size of HardDrive
        },
        0.6,  // tasks' connectivity coefficient
        9,    // priority of task
        60,    // finished time for 1 task
        20    // size of task
    };
    db.tasks.push_back(t6);

    Task t7 = {
        Task::generateId(),
        Task::WAITING,
        5, // the number of tasks
        {
            "x86",
            "Windows 11",
            1,  // the number of processors
            10, // processors' speed
            8,  // RAM size
            512 // available size of HardDrive
        },
        0.6,  // tasks' connectivity coefficient
        5,    // priority of task
        30,   // finished time for 1 task
        20    // size of task
    };
    db.tasks.push_back(t7);
}

void fillResources3(DataBase& db)
{
    Resource r1 = {
        Resource::generateId(),
        {
            "x86",
            "Windows 10",
            1,  // the number of processors
            10, // processors' speed
            64,  // RAM size
            3000 // available size of HardDrive
        },
        10, // channel's bandwith
    };
    db.availableResources.push_back(r1);

    Resource r2 = {
        Resource::generateId(),
        {
            "x86",
            "Windows 10",
            3,  // the number of processors
            10, // processors' speed
            300,  // RAM size
            2000 // available size of HardDrive
        },
        10, // channel's bandwith
    };
    db.availableResources.push_back(r2);
    
    Resource r3 = {
        Resource::generateId(),
        {
            "x86",
            "Windows 11",
            10,  // the number of processors
            10, // processors' speed
            80,  // RAM size
            4000 // available size of HardDrive
        },
        5, // channel's bandwith
    };
    db.availableResources.push_back(r3);

    Resource r4 = {
        Resource::generateId(),
        {
            "x86",
            "Windows 10",
            1,  // the number of processors
            10, // processors' speed
            64,  // RAM size
            3000 // available size of HardDrive
        },
        10, // channel's bandwith
    };
    db.availableResources.push_back(r4);
    
    Resource r5 = {
        Resource::generateId(),
        {
            "x86",
            "Windows 11",
            10,  // the number of processors
            10, // processors' speed
            80,  // RAM size
            4000 // available size of HardDrive
        },
        5, // channel's bandwith
    };
    db.availableResources.push_back(r5);
    
    Resource r6 = {
        Resource::generateId(),
        {
            "x86",
            "Windows 11",
            10,  // the number of processors
            10, // processors' speed
            80,  // RAM size
            4000 // available size of HardDrive
        },
        5, // channel's bandwith
    };
    db.availableResources.push_back(r6);
}
