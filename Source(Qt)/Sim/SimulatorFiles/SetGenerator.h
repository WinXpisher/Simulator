#pragma once

#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include "SimulatorPrimitives.h"

// A generator of incoming sets (tasks and resources)
class SetGenerator
{
private:
    std::vector<std::string> procArchs;
    std::vector<std::string> osTypes;

    int getRandomNumber(int min, int max) {
        return min + rand() % (max - min + 1);
    }

    double roundToDigits(double x, int digits)
    {
        double factor = std::pow(10.0, digits);
        return std::round(x * factor) / factor;
    }

    void fillTask(Task& task)
    {
        std::string arch = procArchs[getRandomNumber(0, procArchs.size() - 1)];
        std::string os = osTypes[getRandomNumber(0, osTypes.size() - 1)];

        task = Task{
            Task::generateId(),
            Task::WAITING,
            getRandomNumber(1, 15), // amount of tasks (in the range from 1 to 15)
            {
                arch,      // processor architecture
                os,        // operation system
                getRandomNumber(1, 10), // amount of processors (in the range from 1 to 10)
                getRandomNumber(1, 20), // processor`s speed (in the range from 1 to 20)
                getRandomNumber(1, 64), // RAM size (in the range of 1 to 64 GB)
                getRandomNumber(10, 512) // available HardDrive size (in the range from 10 to 512 GB)
            },
            // Tasks` connectivity coefficient (from 0.0 to 1.0)
            roundToDigits(static_cast<double>(getRandomNumber(0, 10)) / 10, 1),
            getRandomNumber(1, 10), // tasks priority (in the range from 1 to 10)
            static_cast<double>(getRandomNumber(1, 60)), // running time for 1 task (in the range from 1 to 60 mins)
            static_cast<double>(getRandomNumber(5, 100))  // task size (in the range from 5 to 100 Mb)
        };
    }

    void fillResource(Resource& res)
    {
        std::string arch = procArchs[getRandomNumber(0, procArchs.size() - 1)];
        std::string os = osTypes[getRandomNumber(0, osTypes.size() - 1)];

        res = Resource{
            Resource::generateId(),
            {
                arch,      // processor architecture
                os,        // operation system
                getRandomNumber(1, 50), // amount of processors (in the range from 1 to 50)
                getRandomNumber(5, 40), // processor`s speed (in the range from 5 to 40)
                getRandomNumber(8, 1024), // RAM size (in the range of 8 to 1024 GB)
                getRandomNumber(256, 3000) // available HardDrive size (in the range from 256 to 3000 GB)
            },
            static_cast<double>(getRandomNumber(5, 60)) // Bandwith of channel (in the range from 5 to 60 Mb/mins)
        };
    }
public:
    SetGenerator(
        const std::vector<std::string>& procArchs,
        const std::vector<std::string>& osTypes
    ) : procArchs(procArchs), osTypes(osTypes)
    {
        // init generation of random numbers
        std::srand(std::time(0));
    }

    void fillRandomTasks(vector<Task>& tasks, int count)
    {
        for (int i = 0; i < count; ++i)
        {
            Task task;
            fillTask(task);
            tasks.push_back(task);
        }
    }

    void fillRandomResources(vector<Resource>& resources, int count)
    {
        for (int i = 0; i < count; ++i)
        {
            Resource res;
            fillResource(res);
            resources.push_back(res);
        }
    }
};
