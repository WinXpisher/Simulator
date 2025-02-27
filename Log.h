#pragma once
#include <iostream>  
#include <sstream>
#include <string>  
#include <ctime>
#include <fstream>
#include "SimulationData.h"
#include "SimulatorPrimitives.h" 
#include "DistributionMethod.h"
#include "FileManager.h"

class Logger
{
private:
    using DM = DistributionMethod;
    FileManager fm;
    std::string timeUnit;

    std::string getDistributionMethodName(DM::DMethod dMethod)
    {
        std::string name;

        switch (dMethod)
        {
        case DistributionMethod::FCFS:
            name = "fcfs";
            break;
        case DistributionMethod::LIFO:
            name = "lifo";
            break;
        case DistributionMethod::HPF:
            name = "hpf";
            break;
        case DistributionMethod::BACKFILL:
            name = "backfill";
            break;
        case DistributionMethod::SIMPLEX:
            name = "simplex";
            break;
        case DistributionMethod::RR:
            name = "rr";
            break;
        default:
            name = "unknown";
        }
        return name;
    }

    std::string getCurrentTime()
    {
        time_t now = time(nullptr);
        tm ltm;
        localtime_s(&ltm, &now);

        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(4) << (1900 + ltm.tm_year) << "-"
            << std::setw(2) << (1 + ltm.tm_mon) << "-"
            << std::setw(2) << ltm.tm_mday << " "
            << std::setw(2) << ltm.tm_hour << ":"
            << std::setw(2) << ltm.tm_min << ":"
            << std::setw(2) << ltm.tm_sec;

        return oss.str();
    }
public:
    Logger(std::string timeUnit) : fm(), timeUnit(timeUnit) {}
	~Logger() {}

    void selectDMethod(DM::DMethod dMethod)
    {
        fm.selectDir(getDistributionMethodName(dMethod));
    }

    void logSimulationData(const SimulationData& simData)
    {
        std::ostringstream simDataString;
        simDataString << getCurrentTime()
            << std::fixed << std::setprecision(0)
            << "\t[!] Clock: " << simData.simulationClock << " " << timeUnit
            << std::setprecision(2)
            << " | Average waiting time: " << simData.avWaitTime << " " << timeUnit
            << " | Resource stagnation: " << simData.resStagnation << "%\n";
        fm.writeString("log.txt", simDataString.str());
    }

    void logTaskSendingPool(const Task& task)
    {
        std::ostringstream stringToLog;
        stringToLog << getCurrentTime()
            <<"\t[SEND] Task [" << task.id << "] "
            << "has been added to the sending pool.\n";
        fm.writeString("log.txt", stringToLog.str());
    }

    void logTaskSentToRes(const Task& task, const Resource& res)
    {
        std::ostringstream stringToLog;
        stringToLog << getCurrentTime()
            << "\t[RUN] Task [" << task.id << "] "
            << "started running on resource " << res.id
            << ".\n";
        fm.writeString("log.txt", stringToLog.str());
    }

    void logTaskPerformed(const Task& task)
    {
        std::ostringstream stringToLog;
        stringToLog << getCurrentTime()
            << "\t[PERFORMED] Task [" << task.id << "] "
            << "has been performed.\n";
        fm.writeString("log.txt", stringToLog.str());
    }
};