#include <iostream>
#include <vector>
#include <mutex>
#include <chrono>
#include <atomic>

#include "SimulatorPrimitives.h"
#include "SimulationEnvironment.h"
#include "DistributionMethod.h"
#include "ResourceManager.h"
#include "Log.h"
#include "ConsoleUserInterface.h"
#include "SimulationTests.h"

using namespace std;

std::atomic<bool> stopThread(false);
void updateScreen(
    SimulationEnvironment& simEnv,
    int waitingTimeMilli
)
{
    while (!stopThread)
    {
        {
            std::lock_guard<std::mutex> lock(simEnv.getDataBaseMutex());
            clearConsole();
            cout << "Clock: " << simEnv.getSimulationClock() <<
                " " << getTimeUnit() << "\n\n";
            
            setPrecision();
            cout << "Resource stagnation: " << 
                simEnv.getResourceStagnationAv() << "%\n";

            cout << "Average waiting time: " <<
                simEnv.getSubTaskWaitingTimeAv() <<
                " " << getTimeUnit() << "\n";
            
            resetPrecision();
            printRanTasks(simEnv.getDataBase());
            printSendingPool(simEnv.getSendingPool());
            printWaitingPool(simEnv.getDataBase());
            printRanTasksOnResources(simEnv.getDataBase());
        }
        std::this_thread::sleep_for(
            std::chrono::milliseconds(waitingTimeMilli)
        );
    }
}

#include "DataSetManager.h"
int main()
{
    DataBase db;
    GeneratedSetInfo setInfo = fillDataBase(db);

    // змінні налаштування симуляції
    const int channelCount = 8; // кількість каналів зв'язку
    const int timeUnitToSkip = 1; // скільки одиниць часу буде пропускатися за раз
    const int waitMilliSec = 400; // скільки часу буде затримка
    //const int waitMilliSec = 0; // скільки часу буде затримка
         
    Logger logger(getTimeUnit());

    SimulationEnvironment simEnv(&db, &logger);
    simEnv.prepareForSimulation(channelCount, timeUnitToSkip, waitMilliSec);

    DM* dm = new SIMPLEX();
    logger.selectDMethod(dm->getId());
    logger.logSimulationMetaData(SimulationMetaData{
        setInfo.taskSetNumber,
        setInfo.resSetNumber,
        dm->getId(),
        channelCount
        });

    std::thread simulation([&simEnv, &dm]() {
        simEnv.runSimulation(dm);
     });

    // період оновлення екрану в мілісекундах
    const int updatePeriod = 390;

    std::thread dispatcher(
        updateScreen,
        std::ref(simEnv),
        updatePeriod
    );

    simulation.join();
    stopThread = true;
    dispatcher.join();
    
    delete dm;
    return 0;
}