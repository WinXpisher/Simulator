#ifndef SIMULATIONMAINWIDGET_H
#define SIMULATIONMAINWIDGET_H

#include "SimulatorFiles/SimulatorPrimitives.h"
#include "SimulatorFiles/SimulationEnvironment.h"
#include "TaskTable.h"

#include <QString>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class SimulationMainWidget : public QWidget
{
    Q_OBJECT
public:
    SimulationMainWidget();

    QWidget* getPanel() { return simulationPanel; }

    WaitingTaskTable& getWaitingPool() { return waitingPool; }
    SendingTaskTable& getSendingPool() { return sendingPool; }
    RunningTaskTable& getRunningPool() { return runningPool; }

    void setTimeUnit(QString timeUnitP);

signals:
    void clockChanged(int newValue);
    void stagnationChanged(float newValue);
    void waitingTimeChanged(float newValue);

    void taskAddedToWaitingPool(const Task* task);
    void taskAddedToSendingPool(const SimulationEnvironment::SendingTask* sTask);
    void taskAddedToRunningPool(const Task* task);
    void allPoolsCleared();
public slots:
    void updateClock(int newValue)
    {
        clockLabel->setText(QString("Clock: %1 %2").arg(newValue).arg(timeUnit));
    }

    void updateStagnation(float newValue)
    {
        stagnationLabel->setText(QString("Resource stagnation: %1%").arg(QString::number(newValue, 'f', 2)));
    }

    void updateWaitingTime(float newValue)
    {
        waitingTimeLabel->setText(QString("Average waiting time: %1 %2").arg(QString::number(newValue, 'f', 2)).arg(timeUnit));
    }

    void addTaskToWaitingPool(const Task* task)
    {
        waitingPool.addTask(*task);
    }

    void addTaskToSendingPool(const SimulationEnvironment::SendingTask* sTask)
    {
        sendingPool.addTask(*sTask);
    }

    void addTaskToRunningPool(const Task* task)
    {
        runningPool.addTask(*task);
    }

    void clearAllPools()
    {
        waitingPool.clearTable();
        sendingPool.clearTable();
        runningPool.clearTable();
    }
private:
    WaitingTaskTable waitingPool;
    SendingTaskTable sendingPool;
    RunningTaskTable runningPool;
    QWidget* simulationPanel;

    QLabel *clockLabel;
    QLabel *stagnationLabel;
    QLabel *waitingTimeLabel;
    QString timeUnit;
};

#endif // SIMULATIONMAINWIDGET_H
