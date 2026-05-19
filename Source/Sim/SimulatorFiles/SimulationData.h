#pragma once
#include "DistributionMethod.h"
using DM = DistributionMethod;

// Simulation`s data, which will be used in logs
struct SimulationData
{
    int simulationClock; // general time of simalation
    double avWaitTime; // avarage time of waiting
    double resStagnation; // percentages of resources stagnation
};
