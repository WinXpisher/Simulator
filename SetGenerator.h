#pragma once

#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include "SimulatorPrimitives.h"

// ��������� ������� ������ (������� � �������)
class SetGenerator
{
private:
    std::vector<std::string> procArchs;
    std::vector<std::string> osTypes;

    int getRandomNumber(int min, int max) {
        return min + rand() % (max - min + 1);
    }

    double roundToDigits(double x, int digits)
    {
        double factor = std::pow(10.0, digits);
        return std::round(x * factor) / factor;
    }

    void fillTask(Task& task)
    {
        std::string arch = procArchs[getRandomNumber(0, procArchs.size() - 1)];
        std::string os = osTypes[getRandomNumber(0, osTypes.size() - 1)];

        task = Task{
            Task::generateId(),
            Task::WAITING,
            getRandomNumber(1, 15), // ������� ����� (� ������� �� 1 �� 15)
            {
                arch,      // ����������� ���������
                os,        // ���������� �������
                getRandomNumber(1, 10), // ������� ��������� (�� 1 �� 10)
                getRandomNumber(1, 20), // �������� ��������� (�� 1 �� 20)
                getRandomNumber(1, 64), // ��'�� ���������� ���'�� (�� 1 �� 64 ��)
                getRandomNumber(10, 512) // ��������� ����� ��������� (�� 10 �� 512 ��)
            },
            // ���������� ��'������ ����� � ������� (�� 0.0 �� 1.0)
            roundToDigits(static_cast<double>(getRandomNumber(0, 10)) / 10, 1),
            getRandomNumber(1, 10), // �������� �������� (�� 1 �� 10)
            static_cast<double>(getRandomNumber(1, 60)), // ��� ��������� ���� ������ (�� 1 �� 60 ��)
            static_cast<double>(getRandomNumber(5, 100))  // ����� ������ (�� 5 �� 100 ��)
        };
    }

    void fillResource(Resource& res)
    {
        std::string arch = procArchs[getRandomNumber(0, procArchs.size() - 1)];
        std::string os = osTypes[getRandomNumber(0, osTypes.size() - 1)];

        res = Resource{
            Resource::generateId(),
            {
                arch,      // ����������� ���������
                os,        // ���������� �������
                getRandomNumber(1, 50), // ������� ��������� (�� 1 �� 50)
                getRandomNumber(5, 40), // �������� ��������� (�� 5 �� 40)
                getRandomNumber(8, 1024), // ��'�� ���������� ���'�� (�� 8 �� 1024 ��)
                getRandomNumber(256, 3000) // ��������� ����� ��������� (�� 256 �� 3000 ��)
            },
            static_cast<double>(getRandomNumber(5, 60)) // ��������� �������� ������ (�� 5 �� 60 ��/��)
        };
    }
public:
    SetGenerator(
        const std::vector<std::string>& procArchs,
        const std::vector<std::string>& osTypes
    ) : procArchs(procArchs), osTypes(osTypes)
    {
        // ����������� ���������� ���������� �����
        std::srand(std::time(0));
    }

    void fillRandomTasks(vector<Task>& tasks, int count)
    {
        for (int i = 0; i < count; ++i)
        {
            Task task;
            fillTask(task);
            tasks.push_back(task);
        }
    }

    void fillRandomResources(vector<Resource>& resources, int count)
    {
        for (int i = 0; i < count; ++i)
        {
            Resource res;
            fillResource(res);
            resources.push_back(res);
        }
    }
};