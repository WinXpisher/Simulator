#pragma once
#include "SimulatorPrimitives.h"

class ResourceManager
{
public:
    // Tasks that are running at the moment are occupied by some
    // amount of processors, RAM, etc
    // structure gives the view of the free resource`s component
    struct ResourceRemaining
    {
        int procCount; // Amount of processors
        int memSize; // RAM size
        int discSize; // HardDrive size
    };
    // Method returns the available resource's component that can be occupied.
    // Parameter considerStatus is used by the GUI
    // and if it`s true, only tasks whose status equals tStatus will be in use
    static ResourceRemaining getResourceRemainingData(
        const Resource& res,
        bool considerStatus=false,
        Task::TaskStatus tStatus=Task::TaskStatus::RUNNING
    );
    // How many certain tasks can be run on a certain resource
    static int howManyTasksCanBePerformed(const Task& task, const Resource& res);
    // Whether it can send the task to the resource partially or fully.
    // If the parameter outSubTasksCount is passed and the method returns true,
    // in that address, the number of tasks which can be done by the resource
    // will be recorded.
    static bool canTaskBeSentToResource(
        const Task& task,
        const Resource& res,
        bool areSubTasksConnected,
        int* outSubTasksCount = nullptr
    );
    // Get the minimum time for releasing resource from at least 1 task
    static double getMinTimeToFree(const Resource& res);
    // Find any resource which could run that task
    // partially or filly (depends on the task`s connectivity).
    // If there is none, the method will return nullptr.
    static Resource* findAnyFreeResource(
        const Task& task,
        const vector<Resource*>& resources,
        bool areSubTasksConnected
    );
    // Calculate resource`s stagnation in percents
    static double calcResourceStagnation(vector<Resource>& resources);
    // Find a resource on which the task are running yet
    static const Resource* findResourceTaskIsPerfOn(
        const Task* task,
        const vector<Resource>& resources
    );
};
