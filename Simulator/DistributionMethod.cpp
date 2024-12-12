#include "DistributionMethod.h"

using DM = DistributionMethod;
const DM::DMethod DM::dMethods[1] = { LIFO };

bool LIFO::nextTask(vector<Task>& tasks, Task& outTask) const
{
    // ���� ������� �� ����������, ��������� false
    if (tasks.empty())
        return false;

    // �������� ������ �������� � ������� �����
    // � ��������� ���� � �������
    outTask = tasks.back();
    tasks.pop_back();
    return true;
}