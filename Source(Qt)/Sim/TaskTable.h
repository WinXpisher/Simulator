#ifndef TASKTABLE_H
#define TASKTABLE_H

#include "SimulatorFiles/SimulatorPrimitives.h"
#include "SimulatorFiles/SimulationEnvironment.h"

#include <QString>
#include <QTableWidget>

class TaskTable {
public:
    TaskTable() : timeUnit("")
    {
    }

    void removeTaskById(const QString& id);

    void setTimeUnit(QString timeUnitP) { timeUnit = timeUnitP; }
    void clearTable() { table->setRowCount(0); }
    QTableWidget* getTaskTable() { return table; }
protected:
    QTableWidget *table;
    QString timeUnit;

    QString getTimeUnit()
    {
        return timeUnit;
    }

    QString getTaskPerformTime(const Task& task);
};

class WaitingTaskTable : public TaskTable
{
public:
    WaitingTaskTable();

    void addTask(const Task& task);
};

class SendingTaskTable : public TaskTable
{
public:
    SendingTaskTable();

    void addTask(const SimulationEnvironment::SendingTask& stask);
};

class RunningTaskTable : public TaskTable
{
public:
    RunningTaskTable();

    void addTask(const Task& task);
};

#endif // TASKTABLE_H
