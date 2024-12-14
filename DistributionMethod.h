#pragma once

#include <vector>
#include "SimulatorPrimitives.h"
#include "TaskAnalizer.h"

using namespace std;
class DistributionMethod
{
public:
    enum DMethod
    {
        FCFS,
        LIFO,
        HPF,
        BACKFILL,
        SIMPLEX,
        SMART,
    };
    // масив доступних методів для їх перебору 
    static const DMethod dMethods[1];
private:
    DMethod methodId;
public:
    DistributionMethod(DMethod methodId) : methodId(methodId) {}
    // метод витягує наступне завдання з вектору і записує його в зовнішню змінну,
    // враховуючи метод розподілу, та видаляє його з цього вектору
    // метод повертає false, якщо завдань більше немає, інакше - true
    virtual bool nextTask(
        vector<TaskAnalizer::AnalizerResult>& tasks,
        Task& outTask,
        Resource* outRes=nullptr
    ) const = 0;
    DMethod getId() const { return methodId; }
};

class LIFO : public DistributionMethod
{
private:
    using DM = DistributionMethod;
public:
    LIFO(): DM(DM::DMethod::LIFO) {}
    virtual bool nextTask(
        vector<TaskAnalizer::AnalizerResult>& tasks,
        Task& outTask,
        Resource* outRes=nullptr
    ) const override;
};