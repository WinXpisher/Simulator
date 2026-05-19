#pragma once
#include <vector>
#include "SimulatorPrimitives.h"
using namespace std;

// Class analyses tasks and helps to define resources
// on which each task can be run
class TaskAnalizer
{
public:
    struct AnalizerResult
    {
        Task* task; // task itself
        vector<Resource*> resources; // resources on which each task can be run
    };
private:
    struct ConnectivityResult
    {
        Task* task; // task itself
        bool areConnected; // whether is tasks have a connectivity coefficient
    };
    // Result of working out method analizeConnectivity
    vector<ConnectivityResult> conResult;
    // Result of working out method analizeAllTasks
    vector<AnalizerResult> anResult;
    // The same as anResult, but without tasks with Cancelled status
    // will be transferred by distribution methods
    vector<AnalizerResult> anResultClear;
    DataBase* dataBase; // database whith available resources and tasks
    // Method analyses tasks' connectivity and fills the vector conResult
    void analizeConnectivity();
    // Method analyses tasks
    void analizeTask(Task& task, bool isConnected);
    // Whether the task with NO connectivity can be finished on the specific resource.
    // Method doesn't include the number of processors, because non-connectivity tasks
    // can be distributed on the few clusters
    bool canBePerformedSimple(const Task& task, Resource& res);
    // Whether the task with connectivity can be finished on the specific resource.
    // Method includes the number of processors, because tasks with
    // connectivity CANNOT BE distributed on the few clusters
    bool canBePerformedConnected(const Task& task, Resource& res);
    // Clean result from tasks with Cancelled status and
    // result will record into anResultClear.
    // Method is called automatically in analizeAllTasks method
    void clearFromCancelled();
public:
    TaskAnalizer(DataBase* dataBase) : dataBase(dataBase) {}
    // Method defines all resources, on which task can be run and
    // sets Waiting status, if there are no resources,
    // then method sets Cancelled status
    void analizeAllTasks();
    // Whether tasks are connected
    bool areSubTasksConnected(const Task& task);
    vector<AnalizerResult>& getAnalizeResult() { return anResult; }
    vector<AnalizerResult>& getAnalizeResultClear() { return anResultClear; }
};
