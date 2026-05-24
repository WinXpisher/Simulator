#pragma once

#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include "SimulatorPrimitives.h"

// A generator of incoming sets (tasks and resources)
class SetGenerator
{
private:
    std::vector<std::string> procArchs;
    std::vector<std::string> osTypes;

    int getRandomNumber(int min, int max)
    {
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
            getRandomNumber(1, 15), // count
            {
                arch,      // procArch
                os,        // os
                getRandomNumber(1, 10),  // procCount
                getRandomNumber(1, 20),  // procSpeed
                getRandomNumber(1, 64),  // memSize
                getRandomNumber(10, 512) // discSize
            },
            roundToDigits(static_cast<double>(getRandomNumber(0, 10)) / 10, 1), // connectivity
            getRandomNumber(1, 10), // priority
            static_cast<double>(getRandomNumber(1, 60)), // performTime
            static_cast<double>(getRandomNumber(5, 100)), // subTaskSize
            { nullptr, 0.0, 0.0, {} } // SimulationInfo
        };
    }

    void fillResource(Resource& res)
    {
        std::string arch = procArchs[getRandomNumber(0, procArchs.size() - 1)];
        std::string os = osTypes[getRandomNumber(0, osTypes.size() - 1)];

        res = Resource{
            Resource::generateId(),
            {
                arch,      // procArch
                os,        // os
                getRandomNumber(1, 50),   // procCount
                getRandomNumber(5, 40),   // procSpeed
                getRandomNumber(8, 1024), // memSize
                getRandomNumber(256, 3000)// discSize
            },
            static_cast<double>(getRandomNumber(5, 60)), // bandwidth
            {} // performingTasks
        };
    }

    bool isCompatible(const Task& task, const std::vector<Resource>& resources)
    {
        bool isConnected = task.connectivity > 0.3;

        for (const auto& res : resources)
        {
            if (task.resDesc.procArch != res.resDesc.procArch ||
                task.resDesc.os != res.resDesc.os ||
                task.resDesc.procSpeed > res.resDesc.procSpeed ||
                task.resDesc.procCount > res.resDesc.procCount ||
                task.resDesc.memSize > res.resDesc.memSize ||
                task.resDesc.discSize > res.resDesc.discSize)
            {
                continue;
            }

            if (isConnected)
            {
                if ((task.count * task.resDesc.procCount) > res.resDesc.procCount ||
                    (task.count * task.resDesc.memSize) > res.resDesc.memSize ||
                    (task.count * task.resDesc.discSize) > res.resDesc.discSize)
                {
                    continue;
                }
            }

            return true;
        }
        return false;
    }

    void fillCompatibleTask(Task& task, const std::vector<Resource>& resources)
    {
        if (resources.empty())
        {
            fillTask(task);
            return;
        }

        const Resource& target = resources[getRandomNumber(0, resources.size() - 1)];

        std::string arch = target.resDesc.procArch;
        std::string os = target.resDesc.os;

        int count = getRandomNumber(1, 15);
        double connectivity = roundToDigits(static_cast<double>(getRandomNumber(0, 10)) / 10, 1);
        bool isConnected = connectivity > 0.3;

        if (isConnected)
        {
            // min requirements (proc=1, mem=1, disc=10)
            int maxCountForRes = std::min({
                target.resDesc.procCount / 1,
                target.resDesc.memSize / 1,
                target.resDesc.discSize / 10
            });
            if (count > maxCountForRes)
            {
                count = std::max(1, maxCountForRes);
            }
        }

        int maxProcCount = target.resDesc.procCount;
        int maxMemSize = target.resDesc.memSize;
        int maxDiscSize = target.resDesc.discSize;

        if (isConnected)
        {
            maxProcCount = target.resDesc.procCount / count;
            maxMemSize = target.resDesc.memSize / count;
            maxDiscSize = target.resDesc.discSize / count;
        }

        int procs = getRandomNumber(1, std::min(10, maxProcCount));
        int speed = getRandomNumber(1, std::min(20, target.resDesc.procSpeed));
        int ram = getRandomNumber(1, std::min(64, maxMemSize));
        int hdd = getRandomNumber(10, std::min(512, maxDiscSize));

        task = Task {
            Task::generateId(),
            Task::WAITING,
            count,
            { arch, os, procs, speed, ram, hdd },
            connectivity,
            getRandomNumber(1, 10),
            static_cast<double>(getRandomNumber(1, 60)),
            static_cast<double>(getRandomNumber(5, 100)),
            { nullptr, 0.0, 0.0, {} }
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

    void fillRandomResources(std::vector<Resource>& resources, int count)
    {
        for (int i = 0; i < count; ++i)
        {
            Resource res;
            fillResource(res);
            resources.push_back(res);
        }
    }

    void fillRandomTasks(
        std::vector<Task>& tasks,
        int count,
        const std::vector<Resource>& resources,
        double maxIncompatibleRatio = 0.1
        )
    {
        int maxIncompatible = static_cast<int>(count * maxIncompatibleRatio);
        int currentIncompatible = 0;

        for (int i = 0; i < count; ++i)
        {
            Task task;
            fillTask(task);

            if (!resources.empty() && !isCompatible(task, resources))
            {
                if (currentIncompatible < maxIncompatible)
                {
                    currentIncompatible++;
                } else {
                    fillCompatibleTask(task, resources);
                }
            }

            tasks.push_back(task);
        }
    }
};
