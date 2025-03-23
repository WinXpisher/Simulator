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
        MFQS,
        Penguin
    };
    // ����� ��������� ������ ��� �� ��������  
    static const DMethod dMethods[8];
private:
    DMethod methodId;
public:
    DistributionMethod(DMethod methodId) : methodId(methodId) {}
    // ����� ������ �������� �������� � ������� � ������ ���� � ������� �����, 
    // ���������� ����� ��������, �� ������� ���� � ����� ������� 
    // ����� ������� false, ���� ������� ����� ����, ������ - true 
    virtual bool nextTask(
        vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult,
        Resource*& outRes
    ) const = 0;

    virtual Resource* nextResource(
        TaskAnalizer::AnalizerResult& anResult
    ) const { return nullptr; }

    DMethod getId() const { return methodId; }
};

class FCFS : public DistributionMethod
{
private:
    using DM = DistributionMethod;
public:
    FCFS() : DM(DM::DMethod::FCFS) {}
    virtual bool nextTask(
        vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult,
        Resource*& outRes
    ) const override;
};

class LIFO : public DistributionMethod
{
private:
    using DM = DistributionMethod;
public:
    LIFO() : DM(DM::DMethod::LIFO) {}
    virtual bool nextTask(
        vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult,
        Resource*& outRes
    ) const override;
};

class HPF : public DistributionMethod
{
private:
    using DM = DistributionMethod;
public:
    HPF() : DM(DM::DMethod::HPF) {}
    virtual bool nextTask(
        vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult,
        Resource*& outRes
    ) const override;
};

class BACKFILL : public DistributionMethod
{
private:
    using DM = DistributionMethod;
public:
    BACKFILL() : DM(DM::DMethod::BACKFILL) {}
    virtual bool nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult,
        Resource*& outRes
    ) const override;

    virtual Resource* nextResource(
        TaskAnalizer::AnalizerResult& anResult
    ) const override;
};

class SIMPLEX : public DistributionMethod
{
private:
    using DM = DistributionMethod;
public:
    SIMPLEX() : DM(DM::DMethod::SIMPLEX) {}
    virtual bool nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult,
        Resource*& outRes
    ) const override;
};

class SMART : public DistributionMethod
{
private:
    using DM = DistributionMethod;

public:
    SMART() : DM(DM::DMethod::SMART) {}
    virtual bool nextTask(
        vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult,
        Resource*& outRes) const override;
};

//class MFQS : public DistributionMethod
//{
//private:
//    using DM = DistributionMethod;
//    mutable vector<list<TaskAnalizer::AnalizerResult>> priorityQueues; // ����� ��� ������� ������ ����
//    size_t timeQuantum; // ����� ���� ��� �������� ���������
//
//public:
//    // �����������
//    MFQS(size_t levels = 3, size_t timeQuantum = 10)
//        : DM(DM::DMethod::SMART), timeQuantum(timeQuantum)
//    {
//        if (levels == 0)
//            throw std::invalid_argument("Number of levels must be greater than 0.");
//        priorityQueues.resize(levels);
//    }
//
//    virtual bool nextTask(
//        vector<TaskAnalizer::AnalizerResult>& tasks,
//        TaskAnalizer::AnalizerResult& outAnResult,
//        Resource*& outRes) const override;
//
//    void addToQueue(const TaskAnalizer::AnalizerResult& taskResult, size_t level) const
//    {
//        if (level >= priorityQueues.size())
//            level = priorityQueues.size() - 1; // ����������� ��������� �����
//
//        auto& queue = priorityQueues[level];
//        for (const auto& task : queue)
//        {
//            if (task.task->id == taskResult.task->id) // ��������� ����������
//                return;
//        }
//
//        queue.push_back(taskResult); // ������ �������� � �������� �����
//    }
//};

class Penguin : public DistributionMethod
{
private:
    using DM = DistributionMethod;

public:
    Penguin() : DM(DM::DMethod::Penguin) {}

    virtual bool nextTask(
        vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult,
        Resource*& outRes) const override;

    virtual Resource* nextResource(
        TaskAnalizer::AnalizerResult& anResult
    ) const override;
};
