#pragma once
#include "DistributionMethod.h"
using DM = DistributionMethod;

// ��� ���������, �� ���������������� � �����
struct SimulationData
{
    double simulationClock; // ��������� ��� ���������
    double avWaitTime; // ������� ��� ����������
    double resStagnation; // ������� ������� �������
};