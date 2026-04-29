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
    // The array of available methods for their searching
    static const DMethod dMethods[8];
private:
    DMethod methodId;
public:
    DistributionMethod(DMethod methodId) : methodId(methodId) {}
    // Method that pops the next task from the vector and records it into an outer variable,
    // considering the method of distribution, and deletes it from this vector
    // method returns false if we don't have any tasks, otherwise, true
    virtual bool nextTask(
        vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult
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
        TaskAnalizer::AnalizerResult& outAnResult
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
        TaskAnalizer::AnalizerResult& outAnResult
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
        TaskAnalizer::AnalizerResult& outAnResult
    ) const override;
};

class BACKFILL : public DistributionMethod
{
private:
    using DM = DistributionMethod;
public:
    BACKFILL() : DM(DM::DMethod::BACKFILL) {}
    virtual bool nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult
    ) const override;
};

class SIMPLEX : public DistributionMethod
{
private:
    using DM = DistributionMethod;
public:
    SIMPLEX() : DM(DM::DMethod::SIMPLEX) {}
    virtual bool nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult
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
        TaskAnalizer::AnalizerResult& outAnResult
    ) const override;
};

class MFQS : public DistributionMethod {
private:
    using DM = DistributionMethod;
    size_t levels; // Amount of levels of queue
    vector<size_t> timeQuanta; // Timeslots for each levels of queue

public:
    MFQS(size_t levels = 3, vector<size_t> timeQuanta = { 10, 20, 40 })
        : DM(DM::DMethod::MFQS), levels(levels), timeQuanta(timeQuanta) {
        if (timeQuanta.size() != levels) {
            throw invalid_argument("The number of levels mut be equal to number of time slots.");
        }
    }

    virtual bool nextTask(vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult
    ) const override;
};

class Penguin : public DistributionMethod
{
private:
    using DM = DistributionMethod;

public:
    Penguin() : DM(DM::DMethod::Penguin) {}

    virtual bool nextTask(
        vector<TaskAnalizer::AnalizerResult>& tasks,
        TaskAnalizer::AnalizerResult& outAnResult
    ) const override;

    virtual Resource* nextResource(
        TaskAnalizer::AnalizerResult& anResult
    ) const override;
};
