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

#include <sstream>
#include "Log.h"
#include "FileManager.h"
int main()
{
    //Task t1 = {
    //    Task::generateId(),
    //    Task::WAITING,
    //    10, // кількість задач
    //    {
    //        "x86",
    //        "Windows 10",
    //        1,  // кількість процесорів
    //        10, // швидкодія процесорів
    //        8,  // об'єм оперативної пам'яті
    //        512 // доступний обсяг вінчестера
    //    },
    //    0.2,  // коефіцієнт зв'язності задач у завданні
    //    5,    // пріоритет завдання
    //    6,    // час виконання однієї задачі
    //    20    // розмір задачі
    //};
    //Resource r1 = {
    //    Resource::generateId(),
    //    {
    //        "x86",
    //        "Windows 10",
    //        2,  // кількість процесорів
    //        10, // швидкодія процесорів
    //        64,  // об'єм оперативної пам'яті
    //        3000 // доступний обсяг вінчестера
    //    },
    //    10, // пропускна здатність каналу
    //};
    ///*FileManager fm;
    //fm.selectDir("hello/another_one");
    //fm.selectDir("");
    //fm.selectDir("hello2");
    //cout << fm.writeString("log.txt", "the first string");*/
    //Logger l("minutes");
    //SimulationData simData{ 1, 2, 3 };

    ////cout << l.getCurrentTime();
    //l.selectDMethod(DM::LIFO);
    //l.logSimulationData(simData);
    //l.logTaskSendingPool(t1);
    //l.logTaskSentToRes(t1, r1);
    //l.logTaskPerformed(t1);
    //l.selectDMethod(DM::LIFO);
    //return 0;


    DataBase db;

    fillTasks1(db);
    fillResources1(db);

    // змінні налаштування симуляції
    const int channelCount = 2; // кількість каналів зв'язку
    const int timeUnitToSkip = 1; // скільки одиниць часу буде пропускатися за раз
    const int waitMilliSec = 400; // скільки часу буде затримка
    
    Logger logger(getTimeUnit());
    SimulationEnvironment simEnv(&db, &logger);
    simEnv.prepareForSimulation(channelCount, timeUnitToSkip, 100);

    DM* dm = new FCFS();
    std::thread simulation([&simEnv, &dm]() {
        simEnv.runSimulation(dm);
     });

    // період оновлення екрану в мілісекундах
    const int updatePeriod = 300;

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