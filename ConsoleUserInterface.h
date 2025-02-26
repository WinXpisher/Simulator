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
void resetPrecision() { cout << std::defaultfloat; }

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