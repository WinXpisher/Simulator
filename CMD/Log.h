#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <unordered_map>

#include "SimulationData.h"
#include "SimulatorPrimitives.h"
#include "DistributionMethod.h"
#include "FileManager.h"

struct SimulationMetaData
{
    int taskSetNumber;
    int resSetNumber;
    DM::DMethod dMethod;
    int channelCount;
};

class Logger
{
private:
    using DM = DistributionMethod;
    FileManager fm;
    std::string timeUnit;
    // скільки логів вже було створено для методу розподілу
    std::unordered_map<DM::DMethod, int> logCountForDMethod;
    DM::DMethod selectedDMethod;

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
        case DistributionMethod::Penguin:
            name = "penguin";
            break;
        case DistributionMethod::SMART:
            name = "smart";
            break;
        case DistributionMethod::MFQS:
            name = "mfqs";
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

    std::string getLogFileName(int fileNumber)
    {
        std::string prefix = "log";
        std::string extension = ".txt";
        return prefix + std::to_string(fileNumber) + extension;
    }

    void initDMethodDirectory(DM::DMethod dMethod)
    {
        if (!logCountForDMethod.count(dMethod))
            logCountForDMethod[dMethod] = fm.getNextFileNumber("log", ".txt");
    }
public:
    Logger(std::string timeUnit) : fm(), timeUnit(timeUnit) {}
	~Logger() {}

    void logSimulationMetaData(const SimulationMetaData& metaData)
    {
        std::ostringstream metaDataString;
        metaDataString << "BEGIN Metadata\n"
            << "Simulation started at: " << getCurrentTime() << "\n"
            << "Task set number: " << metaData.taskSetNumber << "\n"
            << "Resource set number: " << metaData.resSetNumber << "\n"
            << "Distribution method: " << getDistributionMethodName(
                metaData.dMethod) << "\n"
            << "Number of channels: " << metaData.channelCount << "\n"
            << "END\n\n";

        fm.writeString(
            getLogFileName(logCountForDMethod[selectedDMethod]),
            metaDataString.str());
    }

    void selectDMethod(DM::DMethod dMethod)
    {
        selectedDMethod = dMethod;
        fm.selectDir("");
        fm.selectDir(getDistributionMethodName(dMethod));
        initDMethodDirectory(dMethod);
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

        fm.writeString(
            getLogFileName(logCountForDMethod[selectedDMethod]), 
            simDataString.str()
        );
    }

    void logTaskSendingPool(const Task& task)
    {
        std::ostringstream stringToLog;
        stringToLog << getCurrentTime()
            <<"\t[SEND] Task [" << task.id << "] "
            << "has been added to the sending pool.\n";

        fm.writeString(
            getLogFileName(logCountForDMethod[selectedDMethod]),
            stringToLog.str()
        );
    }

    void logTaskSentToRes(const Task& task, const Resource& res)
    {
        std::ostringstream stringToLog;
        stringToLog << getCurrentTime()
            << "\t[RUN] Task [" << task.id << "] "
            << "started running on resource " << res.id
            << ".\n";

        fm.writeString(
            getLogFileName(logCountForDMethod[selectedDMethod]),
            stringToLog.str()
        );
    }

    void logTaskPerformed(const Task& task)
    {
        std::ostringstream stringToLog;
        stringToLog << getCurrentTime()
            << "\t[PERFORMED] Task [" << task.id << "] "
            << "has been performed.\n";

        fm.writeString(
            getLogFileName(logCountForDMethod[selectedDMethod]),
            stringToLog.str()
        );
    }

    void logTaskCancelled(const Task& task)
    {
        std::ostringstream stringToLog;
        stringToLog << getCurrentTime()
            << "\t[CANCELLED] Task [" << task.id << "] "
            << "has been cancelled.\n";

        fm.writeString(
            getLogFileName(logCountForDMethod[selectedDMethod]),
            stringToLog.str()
        );
    }
};