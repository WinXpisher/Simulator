#pragma once
#include <string>
#include <vector>
#include <list>

#include <fstream>

using namespace std;
// Resource descriptor
struct ResourceDescriptor
{
    string procArch; // processor architecture
    string os; // operating system
    int procCount; // the number of processors
    int procSpeed; // processors' speed
    int memSize; // RAM size
    int discSize; // available size of HardDrive

    friend std::istream& operator>>(
        std::istream& is,
        ResourceDescriptor& resDesc
    );
    friend std::ostream& operator<<(
        std::ostream& os,
        const ResourceDescriptor& resDesc
    );
};

struct Task;
struct Resource
{
private:
    // Resources have literal IDs
    static std::string globalId;
public:
    // Generate literal IDs in the following format: A-Z, AA-ZZ, AAA-ZZZ, etc
    static std::string generateId();
    
    std::string id;
    ResourceDescriptor resDesc; // resource's descriptor
    double bandwidth; // channels' bandwith (from broker to resource)
    // Tasks, which are running at the moment time
    // if the vector is empty, then the tasks isn't exist
    vector<Task*> performingTasks;

    friend std::istream& operator>>(
        std::istream& is,
        Resource& res
        );
    friend std::ostream& operator<<(
        std::ostream& os,
        const Resource& res
        );
};

// Tasks' information related to simulation
struct SimulationInfo
{
    // Pointer on parent task (for non-child tasks is set to nullptr)
    Task* parentTaskPtr;
    double waitingTime; // how much time is waiting
    double timePerformed; // how much time is task running
    // Child tasks (if the task has a low connectivity coefficient, then
    // the task could be separated into parts to separate into a few resources).
    // Use list to guarantee that memory won't be redistributed.
    std::list<Task> childTasks;
};

// Task that includes package of tasks
struct Task
{
private:
    // Tasks have a digit IDs
    static int globalId;
public:
    // The method calculates the number of undistributed tasks
    // remaining on the resources. Get the difference between
    // the number of tasks parentTask and the sum of amount of subtasks.
    static int getRemainingSubTasksCount(const Task& parentTask);
    // Generate the digit ID
    static std::string generateId();

    enum TaskStatus
    {
        WAITING,   // task is waiting
        CANCELLED, // task is cancelled
        SENDING,   // task is sending on the resource
        RUNNING,   // task is running
        PERFORMED, // task is perfomed
        // Tasks are separated into small tasks (child tasks),
        // but his part is waiting yet
        DIVIDED,
        // Tasks are separated into small tasks (child tasks)
        // and all these child tasks whether they are sending
        // or running (can be both variants), but they aren't waiting
        DIVIDED_RUNNING
        /*
         * If tasks have DIVIDED_RUNNING status and all their
         * child tasks have PERFORMED status, then this task
         * is considered as performed, and it has PERFORMED status.
        */
    };
    std::string id;
    TaskStatus status; // task status
    int count; // the number of tasks
    ResourceDescriptor resDesc; // resource's desription, on which each task can be performed
    double connectivity; // tasks' connectivity coefficient
    int priority; // task priority
    double performTime; // perform time for 1 task
    double subTaskSize; // task size (execution file, which will be sent)
    SimulationInfo simulationInfo; // additional information about the simulation

    friend std::istream& operator>>(
        std::istream& is,
        Task& task
        );
    friend std::ostream& operator<<(
        std::ostream& os,
        const Task& task
        );
};

struct DataBase
{
    vector<Task> tasks; // tasks which must be done
    vector<Resource> availableResources; // available resources
};
