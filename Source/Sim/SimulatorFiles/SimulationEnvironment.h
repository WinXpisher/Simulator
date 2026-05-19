#pragma once
#include <thread>
#include <mutex>
#include <vector>

#include "SimulatorPrimitives.h"
#include "DistributionMethod.h"
#include "TaskAnalizer.h"
#include "SimulationData.h"
#include "Log.h"
using namespace std;

class SimulationEnvironment
{
public:
    struct SendingTask;
private:
    using DM = DistributionMethod;

    // Variables, which are used in the simulation
    struct SimulationContext
    {
        vector<TaskAnalizer::AnalizerResult> anResults; // result of the tasks' analysis
        TaskAnalizer::AnalizerResult anResult; // object, in which are saving present task
        bool hasTask; // whether it has any tasks in pool of tasks
        int subTasksRemain; // the number of tasks that are left
        bool areSubTasksConnected; // whether subtasks are connected with tasks
        double timeUnitToSkip; // time unit for skipping
        int waitMilliSec; // waiting time in ms
        bool actionTaken; // whether action is finished successfully in the present iteration
    };
    SimulationContext simContext;

    std::mutex dataBaseMutex;
    Logger* logger;

    vector<SendingTask> sendingPool; // pool of tasks, which are sent to a resource
    TaskAnalizer taskAnalizer; // object, which will be analysed for each task before simulation
    vector<DM*> distributionMethods; // distribution methods
    DataBase* dataBase; // database for available resources and tasks
    
    int channelCount; // the number of communications channel
    double resourceStagnationSum; // sum of the resources' stagnation in percents
    double subTaskWaitingTimeSum; // sum of waiting time for tasks
    int simulationClock; // simulation clock

    // Initialization of variables before simulation
    void initSimContext();
    // Initialization of tasks' information, which are connected with simulation
    void initTasksSimulationInfo();
    // Method simmulates waiting time for resources
    void waitForResources(double time, vector<Resource>& resources);
    // Method simulates waiting time for a pool of sending tasks
    void waitForSendingPool(double time);
    // Whether all tasks are finished
    bool haveAllTasksPerformed();
    // Method simulates waiting time on the program level
    void modelWaiting(double time);
    // Method simulates waiting time for tasks, which are in queue
    // and updates their attribute waitingTime
    void modelWaitingForSubTasks(double time);
    // Get general number of tasks
    int getSubTasksCount() const;
    // Method calculates general waiting time for tasks on current
    // iteration and saves result in subTaskWaitingTimeSum
    void calcSubTaskWaitingTimeCurrent();
    // Method waits on physical level
    void wait(int time);
    // Method distributes tasks on the resource, if it is possible.
    // If the task was distributed fully, the method sets the SENDING status
    // and adds it to the pool of sending. If sent can be only part of the task,
    // it is distributed into small tasks (or subtasks), which are set SENDING status,
    // but for parent task is set DEVIDED or DEVIDED_RUNNING status,
    // depends on whether the task was distributed fully or if some part is waiting
    // in the pool of tasks.
    // Return value is the number of distributed tasks.
    int trySendTaskToResource(
        Task& task,
        Resource& resource, 
        bool areSubTasksConnected
    );
    // Get a copy of the task, but from the general number of tasks will be
    // minus the sum of the number of subtasks (current task must be separated).
    // For the tasks with status, which are not DIVIDED, the method returns a copy without changes.
    Task getTaskCopyWithRemainingCount(const Task& task);
    // Method sets the status of a separated task depending on
    // whether all tasks are distributed on the resources (even if they
    // are staying in the pool of sending). If it is true, then
    // it sets DIVIDED_RUNNING status; otherwise, it just DIVIDED
    void setDividedTaskStatus(Task& parentTask);
    // Method finishes the parent task (if it exists) of the current task,
    // if the distribution method itself has DIVIDED_RUNNING status and
    // all its subtasks have PERFORMED status
    void finishDividedTaskIfNeed(Task& childTask);
    // Get the delay in network while finding tasks to cluster
    // subTasksCount - the number of tasks, which will be sent
    double getNetworkDelay(
        const Task& task,
        const Resource& res,
        int subTasksCount);
public:
    // Task, which is sending on the resource
    struct SendingTask
    {
        Task* task;
        float timeToSend; // time, which is left to send task
    };

    SimulationEnvironment(DataBase* dataBase, Logger* logger);
    ~SimulationEnvironment();

    // Method prepares data for simulation beginning
    void prepareForSimulation(
        int channelCount=1,
        int timeUnitToSkip=1,
        int waitMilliSec=400
    );
    // Method runs a simulation for 1 specific distribution method
    void runSimulation(const DM* dm);
    
    int getSimulationClock() const { return simulationClock; }
    const DataBase& getDataBase() const { return *dataBase; }
    std::mutex& getDataBaseMutex() { return dataBaseMutex; }
    const vector<SendingTask>& getSendingPool() const { return sendingPool; }
    // Get avarage percent of recources stagnation
    const double getResourceStagnationAv() const
    { 
        return (simulationClock == 0) ? 0 :
            resourceStagnationSum / simulationClock;
    }

    // Get avarage waiting time
    const double getSubTaskWaitingTimeAv() const
    {
        int subTasksCount = getSubTasksCount();
        return (subTasksCount == 0) ? 0 :
            subTaskWaitingTimeSum / subTasksCount;
    }
};
