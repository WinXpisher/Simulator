#pragma once
#include <iostream>
#include <vector>
#include "SimulatorPrimitives.h"
#include "SimulationEnvironment.h"
#include "ResourceManager.h"

std::string getTaskStatus(const Task& task)
{
    switch (task.status)
    {
    case Task::WAITING:
        return "WAITING";
    case Task::CANCELLED:
        return "CANCELLED";
    case Task::SENDING:
        return "SENDING";
    case Task::RUNNING:
        return "RUNNING";
    case Task::PERFORMED:
        return "PERFORMED";
    case Task::DIVIDED:
        return "DIVIDED";
    case Task::DIVIDED_RUNNING:
        return "DIVIDED_RUNNING";
    default:
        return "UNKNOWN";
    }
}

#include <iomanip>
void setPrecision() { cout << std::fixed << std::setprecision(2); }
void resetPrecision(int significantDigits = 3)
{
    cout.unsetf(std::ios::fixed | std::ios::scientific); 
    cout.precision(significantDigits);
}

void clearConsole() { std::system("cls"); }

string getSeparator()
{
    return "\t";
}

string getTimeUnit()
{
    return "minutes";
}

void printTaskMainInfo(const Task& task)
{
    cout << task.id << getSeparator() <<
        getTaskStatus(task);
}

void printTaskPerformTime(const Task& task)
{
    if (task.simulationInfo.childTasks.empty())
    {
        cout << task.simulationInfo.timePerformed << "/" <<
            task.performTime << " " << getTimeUnit();
    }
    else
    {
        double totalPerformedTime = 0;
        double totalTaskTime = 0;

        for (const Task& child : task.simulationInfo.childTasks)
        {
            totalPerformedTime += child.simulationInfo.timePerformed;
            totalTaskTime += child.performTime;
        }

        float percent = (totalTaskTime > 0) ?
            (totalPerformedTime / totalTaskTime) * 100 : 0;
        setPrecision();
        cout << percent << "%";
        resetPrecision();
    }
}

void printSendingTask(const SimulationEnvironment::SendingTask& sTask)
{
    /*if (sTask.task->id == "9.2")
    {
        cout << sTask.timeToSend << "\n";
    }*/

    printTaskMainInfo(*sTask.task);
    cout << getSeparator() << "remain " <<
        sTask.timeToSend << " " << getTimeUnit() <<
        "\n";
}


void printWaitingPool(const DataBase& db)
{
    cout << "Waiting pool:\n";
    for (const Task& task : db.tasks)
    {
        if (task.status == Task::TaskStatus::DIVIDED)
        {
            printTaskMainInfo(task);
            cout << getSeparator() << "distributed: " <<
                task.count - Task::getRemainingSubTasksCount(task) <<
                "/" << task.count << " subtasks";
            cout << "\n";
        }
    }
    for (const Task& task : db.tasks)
    {
        if (task.status == Task::TaskStatus::WAITING)
        {
            printTaskMainInfo(task);
            cout << "\n";
        }
    }
    for (const Task& task : db.tasks)
    {
        if (task.status == Task::TaskStatus::CANCELLED)
        {
            printTaskMainInfo(task);
            cout << "\n";
        }
    }
    cout << "\n";
}

void printRanTasks(const DataBase& db)
{
    cout << "Running tasks:\n";
    for (const Task& task : db.tasks)
    {
        if (task.status == Task::TaskStatus::RUNNING ||
            task.status == Task::TaskStatus::DIVIDED_RUNNING)
        {
            printTaskMainInfo(task);
            cout << getSeparator();
            printTaskPerformTime(task);
            cout << "\n";
        }
    }
    for (const Task& task : db.tasks)
    {
        if (task.status == Task::TaskStatus::PERFORMED)
        {
            printTaskMainInfo(task);
            cout << getSeparator();
            printTaskPerformTime(task);
            cout << "\n";
        }
    }
    for (const Task& task : db.tasks)
    {
        for (const Task& child : task.simulationInfo.childTasks)
        {
            if (child.status == Task::TaskStatus::RUNNING ||
                child.status == Task::TaskStatus::DIVIDED_RUNNING ||
                child.status == Task::TaskStatus::PERFORMED)
            {
                printTaskMainInfo(child);
                cout << getSeparator();
                printTaskPerformTime(child);
                cout << "\n";
            }
        }
    }
    cout << "\n";
}

void printResource(const Resource& res)
{
    ResourceManager::ResourceRemaining rr =
        ResourceManager::getResourceRemainingData(res, true);
    int busyProc = res.resDesc.procCount - rr.procCount;

    cout << res.id << getSeparator() <<
        res.resDesc.procArch << getSeparator() <<
        res.resDesc.os << getSeparator() <<
        busyProc << "/" << res.resDesc.procCount << " processing units";
}

void printRanTasksOnResources(const DataBase& db)
{
    cout << "Resources:\n";
    for (const Resource& res : db.availableResources)
    {
        printResource(res);
        cout << "\n";
        for (const Task* task : res.performingTasks)
        {
            if (task->status == Task::TaskStatus::RUNNING ||
                task->status == Task::TaskStatus::DIVIDED_RUNNING
                )
            {
                printTaskMainInfo(*task);
                cout << getSeparator();
                printTaskPerformTime(*task);
                cout << "\n";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

void printSendingPool(
    const vector<SimulationEnvironment::SendingTask>& sendingPool
)
{
    cout << "Sending pool:\n";
    for (const SimulationEnvironment::SendingTask& sTask : sendingPool)
    {
        printSendingTask(sTask);
    }
    cout << "\n";
}

#include "SetGenerator.h"
#include "DataSetManager.h"
#include <limits>  // std::numeric_limits

const std::vector<std::string> procArchs = { "x86" };
const std::vector<std::string> osTypes = { "Windows", "Linux" };

int getNumberFromUser()
{
    int number;
    while (true)
    {
        if (std::cin >> number)
            return number;
        else
        {
            std::cout << "Invalid input. Please enter a valid number.\n";

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

struct GeneratedSetInfo
{
    int taskSetNumber;
    int resSetNumber;
};

GeneratedSetInfo fillDataBase(DataBase& db)
{
    DataSetManager dsm;
    SetGenerator sg(procArchs, osTypes);
    std::string answer;

    GeneratedSetInfo setInfo;
    while (true)
    {
        cout << "What do you want to do, generate a" << 
            " new data set or choose an existing one (generate/choose): ";
        std::getline(cin, answer);

        if (answer == "generate")
        {
            cout << "How many tasks to generate: ";
            int tasksCount = getNumberFromUser();
            sg.fillRandomTasks(db.tasks, tasksCount);

            cout << "How many resources to generate: ";
            int resCount = getNumberFromUser();
            sg.fillRandomResources(db.availableResources, resCount);

            setInfo.taskSetNumber = dsm.writeTasks(db.tasks);
            setInfo.resSetNumber = dsm.writeResources(db.availableResources);
            break;
        }
        else if (answer == "choose")
        {
            cout << "Choose data set number: ";
            int DSNumber = getNumberFromUser();
            dsm.loadTasks(db.tasks, DSNumber);
            dsm.loadResources(db.availableResources, DSNumber);
            
            setInfo.taskSetNumber = DSNumber;
            setInfo.resSetNumber = DSNumber;
            break;
        }
        else
        {
            cout << "Invalid option. Please type 'generate' or 'choose'.\n";
        }
    }
    return setInfo;
}
