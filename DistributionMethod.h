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
        RR,
    };
    // ����� ��������� ������ ��� �� ��������  
    static const DMethod dMethods[6];
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

class RR : public DistributionMethod
{
private:
    using DM = DistributionMethod;
    mutable int currentIndex = 0; // ��� ���� ��� ����� ���� ������������ �� 133 ����� � .cpp ���� ���������
    size_t quantum;
public:
    RR(size_t quantum) : DM(DM::DMethod::RR), quantum(quantum) {}
    virtual bool nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult,
        Resource*& outRes
    ) const override;
};