#include "DistributionMethod.h"

using DM = DistributionMethod;
const DM::DMethod DM::dMethods[1] = { LIFO };

bool LIFO::nextTask(
    vector<TaskAnalizer::AnalizerResult>& tasks,
    Task& outTask,
    Resource* outRes) const
{
    // ���� ������� �� ����������, ��������� false
    if (tasks.empty())
        return false;

    // �������� ������ �������� � ������� �����
    // � ��������� ���� � �������
    outTask = *tasks.back().taskPtr;
    tasks.pop_back();
    return true;
}