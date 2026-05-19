#ifndef RESOURCESDOCKWIDGET_H
#define RESOURCESDOCKWIDGET_H

#include "SimulatorFiles/SimulatorPrimitives.h"
#include <QDockWidget>
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class ResourcesDockWidget: public QWidget
{
    Q_OBJECT
public:
    ResourcesDockWidget();

    void addResource(const Resource& res);
    void addTask(const Task& task);

    QDockWidget* getDock() { return dock; }
private:
    QString resToString(const Resource& res);
    QString taskToString(const Task& task);

    // void clearResTree() { resTree->clear(); }
private:
    QTreeWidget *resTree;
    QTreeWidgetItem *currentItem;
    QDockWidget *dock;
signals:
    void resAddedToResPannel(const Resource* res);
    void taskAddedToResPannel(const Task* task);
    void resTreeCleared();
public slots:
    void addResToResPool(const Resource* res)
    {
        addResource(*res);
    }

    void addTaskToResPool(const Task* task)
    {
        addTask(*task);
    }

    void clearResTree()
    {
        resTree->clear();
    }
};

#endif // RESOURCESDOCKWIDGET_H
