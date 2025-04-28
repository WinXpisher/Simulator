#include "SimulatorFiles/ResourceManager.h"
#include "ResourcesDockWidget.h"
#include "GetTaskStatusStr.h"

#include <QLabel>
#include <QTreeWidget>

ResourcesDockWidget::ResourcesDockWidget()
{
    dock = new QDockWidget();
    QLabel *dockTitleWidget = new QLabel("Resources");
    dockTitleWidget->setStyleSheet("font-weight: bold; font-size: 14px; color: rgb(255, 128, 0);");
    dock->setTitleBarWidget(dockTitleWidget);
    resTree = new QTreeWidget(dock);
    resTree->setColumnCount(1);
    resTree->setHeaderHidden(true);

    dock->setWidget(resTree);

    connect(this, &ResourcesDockWidget::resAddedToResPannel, this, &ResourcesDockWidget::addResToResPool);
    connect(this, &ResourcesDockWidget::taskAddedToResPannel, this, &ResourcesDockWidget::addTaskToResPool);
    connect(this, &ResourcesDockWidget::resTreeCleared, this, &ResourcesDockWidget::clearResTree);
}

void ResourcesDockWidget::addResource(const Resource& res)
{
    currentItem = new QTreeWidgetItem(resTree);
    currentItem->setText(0, resToString(res));
}

void ResourcesDockWidget::addTask(const Task& task)
{
    QTreeWidgetItem *child = new QTreeWidgetItem();
    child->setText(0, taskToString(task));
    currentItem->addChild(child);
    resTree->expandAll();
}

QString ResourcesDockWidget::resToString(const Resource& res)
{
    QString result;

    result += QString::fromStdString(res.id);
    result += QString("\t") + QString::fromStdString(res.resDesc.procArch);
    result += QString("\t") + QString::fromStdString(res.resDesc.os);

    ResourceManager::ResourceRemaining rr =
        ResourceManager::getResourceRemainingData(res, true);
    int busyProc = res.resDesc.procCount - rr.procCount;
    result += QString("\t%1/%2 processing units").arg(busyProc).arg(res.resDesc.procCount);

    return result;
}

QString ResourcesDockWidget::taskToString(const Task& task)
{
    return QString("%1\t%2\t%3/%4 minutes")
    .arg(QString::fromStdString(task.id))
        .arg(getTaskStatusStr(task))
        .arg(QString::number(task.simulationInfo.timePerformed, 'f', 0))
        .arg(QString::number(task.performTime, 'f', 0));
}
