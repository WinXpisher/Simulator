#pragma once  
#include <iostream>  
#include <fstream>  
#include <string>  
#include <ctime>  
#include "SimulationData.h"

using namespace std;

class LogInf
{
public:
    LogInf(const string& filename)
    {
        logFile.open(filename, ios::out | ios::app);
        if (!logFile.is_open())
        {
            cerr << "Error: can't open log file" << endl;
        }
    }

    ~LogInf()
    {
        if (logFile.is_open())
        {
            logFile.close();
        }
    }

    void logSimResult(const SimulationData& simData, DM::DMethod methodId)
    {
        if (logFile.is_open())
        {
            logFile << getCurrentTime() << " " << 
                getSimResultString(simData, methodId) << endl;
        }
    }

    string createMessageFromSimResult(
        const SimulationData& simData, DM::DMethod methodId
    )
    {
        return getSimResultString(simData, methodId);
    }

    void saveMessageToTxt(
        const SimulationData& simData, 
        DM::DMethod methodId,
        const string& txtFilename
    )
    {
        ofstream txtFile(txtFilename, ios::out | ios::app);
        if (txtFile.is_open())
        {
            txtFile << getCurrentTime() << " " << 
                getSimResultString(simData, methodId) << endl;
            txtFile.close();
        }
        else
        {
            cerr << "Error: can't open txt file" << endl;
        }
    }

    string formatSimResult(const SimulationData& simData, DM::DMethod methodId)
    {
        return getSimResultString(simData, methodId);
    }

    void logSimResultMessage(const SimulationData& simData, DM::DMethod methodId)
    {
        string message = createMessageFromSimResult(simData, methodId);
        logMessage(message);
    }

    void logMessage(const string& message)
    {
        if (logFile.is_open())
        {
            logFile << getCurrentTime() << " " << message << endl;
        }
    }

private:
    ofstream logFile;

    string getCurrentTime()
    {
        time_t now = time(nullptr);
        tm ltm;
        localtime_s(&ltm, &now);
        return to_string(1900 + ltm.tm_year) + "-" + to_string(1 + ltm.tm_mon) + "-" + to_string(ltm.tm_mday) + " " + to_string(ltm.tm_hour) + ":" + to_string(ltm.tm_min) + ":" + to_string(ltm.tm_sec);
    }

    string getSimResultString(const SimulationData& simData, DM::DMethod methodId)
    {
        string methodName;
        switch (methodId)
        {
        case 1:
            methodName = "Method LIFO";
            break;
        case 2:
            methodName = "Method FCFS";
            break;
        case 3:
            methodName = "Method HPF";
            break;
        case 4:
            methodName = "Method BACKFILL";
            break;
        case 5:
            methodName = "Method SIMPLEX";
            break;
        default:
            methodName = "Method RR";
            break;
        }

        return "Method: " + methodName + "\n" +
            "Execution Time: " + to_string(simData.simulationClock) + "\n" +
            "Average Wait Time: " + to_string(simData.avWaitTime) + "\n";
    }
};
