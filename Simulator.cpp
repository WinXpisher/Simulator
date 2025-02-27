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
    //    10, // ������� �����
    //    {
    //        "x86",
    //        "Windows 10",
    //        1,  // ������� ���������
    //        10, // �������� ���������
    //        8,  // ��'�� ���������� ���'��
    //        512 // ��������� ����� ���������
    //    },
    //    0.2,  // ���������� ��'������ ����� � �������
    //    5,    // �������� ��������
    //    6,    // ��� ��������� ���� ������
    //    20    // ����� ������
    //};
    //Resource r1 = {
    //    Resource::generateId(),
    //    {
    //        "x86",
    //        "Windows 10",
    //        2,  // ������� ���������
    //        10, // �������� ���������
    //        64,  // ��'�� ���������� ���'��
    //        3000 // ��������� ����� ���������
    //    },
    //    10, // ��������� �������� ������
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

    // ���� ������������ ���������
    const int channelCount = 2; // ������� ������ ��'����
    const int timeUnitToSkip = 1; // ������ ������� ���� ���� ������������ �� ���
    const int waitMilliSec = 400; // ������ ���� ���� ��������
    
    Logger logger(getTimeUnit());
    SimulationEnvironment simEnv(&db, &logger);
    simEnv.prepareForSimulation(channelCount, timeUnitToSkip, 100);

    DM* dm = new FCFS();
    std::thread simulation([&simEnv, &dm]() {
        simEnv.runSimulation(dm);
     });

    // ����� ��������� ������ � ����������
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