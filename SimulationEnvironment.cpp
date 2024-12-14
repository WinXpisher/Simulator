#include "SimulationEnvironment.h"

using DM = DistributionMethod;
SimulationEnvironment::SimulationEnvironment(DataBase* dataBase):
    dataBase(dataBase), taskAnalizer(dataBase)
{

}

SimulationEnvironment::~SimulationEnvironment()
{
    delete dataBase;
    dataBase = nullptr;
    for (auto dm : distributionMethods)
        delete dm;
}

void SimulationEnvironment::initDMethods()
{
    for (auto dm : DM::dMethods)
    {
        switch (dm)
        {
        case DM::FCFS:
            break;
        case DM::LIFO:
            distributionMethods.push_back(new LIFO());
            break;
        case DM::HPF:
            break;
        case DM::BACKFILL:
            break;
        case DM::SIMPLEX:
            break;
        case DM::SMART:
            break;
        default:
            break;
        }
    }
}

void SimulationEnvironment::runSimulation()
{
    // проходимо по кожному методу розподілення
    for (DM* dm : distributionMethods)
    {
        //runResultsForDMethod(dm);
    }
}

void SimulationEnvironment::runResultsForDMethod(const DM* dm)
{
    /*vector<Task> tasksCopy = dataBase->tasks;
    Task task;
    while (dm->nextTask(tasksCopy, task))
    {
        
    }*/
}