#pragma once
#include "DistributionMethod.h"
using DM = DistributionMethod;

// ��� ���������, �� ���������������� � �����
struct SimulationData
{
    int simulationClock; // ��������� ��� ���������
    double avWaitTime; // ������� ��� ����������
    double resStagnation; // ������� ������� �������
};