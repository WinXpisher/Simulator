#pragma once
#include "DistributionMethod.h"
using DM = DistributionMethod;

// дані симуляції, які використовуються в логах
struct SimulationData
{
    int simulationClock; // загальний час симуляції
    double avWaitTime; // середній час очікування
    double resStagnation; // відсоток простою ресурсів
};