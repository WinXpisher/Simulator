#ifndef SIMULATIONDISPATCHER_H
#define SIMULATIONDISPATCHER_H

#include <atomic>
#include <mutex>
#include <thread>
#include <QObject>

#include "SimulatorFiles/SimulatorPrimitives.h"
#include "SimulationMainWidget.h"
#include "ResourcesDockWidget.h"
#include "SimulatorFiles/SimulationEnvironment.h"

class SimulationDispatcher: QObject
{
    Q_OBJECT
private:
    DataBase* db;
    SimulationEnvironment* simEnv;
    std::atomic<bool>* stopThread;
    SimulationMainWidget* simulationMainWidget;
    ResourcesDockWidget* resourcesDockWidget;
    int updatePeriod;
public:
    SimulationDispatcher(
        SimulationEnvironment* simEnvPar,
        std::atomic<bool>* stopThreadPar,
        SimulationMainWidget* simulationMainWidgetPar,
        ResourcesDockWidget* resourcesDockWidgetPar,
        DataBase* dbPar,
        int updatePeriodPar) :
            simEnv(simEnvPar), stopThread(stopThreadPar),
            simulationMainWidget(simulationMainWidgetPar),
            resourcesDockWidget(resourcesDockWidgetPar),
            db(dbPar), updatePeriod(updatePeriodPar)
    {

    }

    void Start()
    {
        while (!(*stopThread))
        {
            {
                updateInfo();
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(updatePeriod)
            );
        }
        // Щоб отримати кінцеві дані (якщо вони були пропущені).
        // Це може статися, якщо період в самому симуляторі менше ніж період оновлення вікна.
        updateInfo();
    }
private:
    void updateInfo()
    {
        std::lock_guard<std::mutex> lock(simEnv->getDataBaseMutex());
        int clock = simEnv->getSimulationClock();
        float resourceStagnation = simEnv->getResourceStagnationAv();
        float avWaitingTime = simEnv->getSubTaskWaitingTimeAv();

        emit simulationMainWidget->clockChanged(clock);
        emit simulationMainWidget->stagnationChanged(resourceStagnation);
        emit simulationMainWidget->waitingTimeChanged(avWaitingTime);

        emit simulationMainWidget->allPoolsCleared();
        addWaitingTasks();
        addSendingTasks(simEnv->getSendingPool());
        addRanTasks();

        emit resourcesDockWidget->resTreeCleared();
        fillResourcesTreePanel();
    }

    void addWaitingTasks()
    {
        for (const Task& task : db->tasks)
        {
            if (task.status == Task::TaskStatus::DIVIDED)
            {
                emit simulationMainWidget->taskAddedToWaitingPool(&task);
            }
        }
        for (const Task& task : db->tasks)
        {
            if (task.status == Task::TaskStatus::WAITING)
            {
                emit simulationMainWidget->taskAddedToWaitingPool(&task);
            }
        }
        for (const Task& task : db->tasks)
        {
            if (task.status == Task::TaskStatus::CANCELLED)
            {
                emit simulationMainWidget->taskAddedToWaitingPool(&task);
            }
        }
    }

    void addSendingTasks(const vector<SimulationEnvironment::SendingTask>& sendingPool)
    {
        for (const SimulationEnvironment::SendingTask& sTask : sendingPool)
        {
            emit simulationMainWidget->taskAddedToSendingPool(&sTask);
        }
    }


    void addRanTasks()
    {
        for (const Task& task : db->tasks)
        {
            if (task.status == Task::TaskStatus::RUNNING ||
                task.status == Task::TaskStatus::DIVIDED_RUNNING)
            {
                emit simulationMainWidget->taskAddedToRunningPool(&task);
            }
        }
        for (const Task& task : db->tasks)
        {
            if (task.status == Task::TaskStatus::PERFORMED)
            {
                emit simulationMainWidget->taskAddedToRunningPool(&task);
            }
        }
        for (const Task& task : db->tasks)
        {
            for (const Task& child : task.simulationInfo.childTasks)
            {
                if (child.status == Task::TaskStatus::RUNNING ||
                    child.status == Task::TaskStatus::DIVIDED_RUNNING ||
                    child.status == Task::TaskStatus::PERFORMED)
                {
                    emit simulationMainWidget->taskAddedToRunningPool(&child);
                }
            }
        }
    }

    void fillResourcesTreePanel()
    {
        for (const Resource& res : db->availableResources)
        {
            emit resourcesDockWidget->resAddedToResPannel(&res);
            for (const Task* task : res.performingTasks)
            {
                if (task->status == Task::TaskStatus::RUNNING ||
                    task->status == Task::TaskStatus::DIVIDED_RUNNING
                    )
                {
                    emit resourcesDockWidget->taskAddedToResPannel(task);
                }
            }
        }
    }
};

#endif // SIMULATIONDISPATCHER_H
