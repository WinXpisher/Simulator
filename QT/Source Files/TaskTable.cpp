#include "TaskTable.h"
#include "SimulatorFiles/SimulatorPrimitives.h"
#include "SimulatorFiles/SimulationEnvironment.h"
#include "GetTaskStatusStr.h"

#include <QTableWidgetItem>
#include <QTableWidget>
#include <QHeaderView>
#include <QAbstractItemView>

void TaskTable::removeTaskById(const QString& id)
{
    for (int row = 0; row < table->rowCount(); ++row)
    {
        QTableWidgetItem *item = table->item(row, 0);
        if (item && item->text() == id)
        {
            table->removeRow(row);
            break;
        }
    }
}

QString TaskTable::getTaskPerformTime(const Task& task)
{
    if (task.simulationInfo.childTasks.empty())
    {
        return QString::number(task.simulationInfo.timePerformed, 'f', 0) + "/" +
               QString::number(task.performTime, 'f', 0) + " " + getTimeUnit();
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

        return QString::number(percent, 'f', 2) + "%";
    }
}


WaitingTaskTable::WaitingTaskTable()
{
    table = new QTableWidget();
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"ID", "Status"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void WaitingTaskTable::addTask(const Task& task)
{
    int row = table->rowCount();

    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(task.id.c_str()));
    table->setItem(row, 1, new QTableWidgetItem(getTaskStatusStr(task)));
}


SendingTaskTable::SendingTaskTable()
{
    table = new QTableWidget();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"ID", "Status", "Remain"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
}

void SendingTaskTable::addTask(const SimulationEnvironment::SendingTask& stask)
{
    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(stask.task->id.c_str()));
    table->setItem(row, 1, new QTableWidgetItem(getTaskStatusStr(*stask.task)));
    table->setItem(row, 2, new QTableWidgetItem(QString::number(stask.timeToSend, 'f', 2) + " " + getTimeUnit()));
}


RunningTaskTable::RunningTaskTable()
{
    table = new QTableWidget();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"ID", "Status", "Progress"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void RunningTaskTable::addTask(const Task& task)
{
    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(task.id.c_str()));
    table->setItem(row, 1, new QTableWidgetItem(getTaskStatusStr(task)));
    table->setItem(row, 2, new QTableWidgetItem(getTaskPerformTime(task)));
}
