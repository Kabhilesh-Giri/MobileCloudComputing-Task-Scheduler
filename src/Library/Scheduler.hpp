#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include "Task.hpp"

class Task;

class Scheduler {
public:
    double findMaxValue(const std::vector<double>&) const;
    double calculatereadyTimeLocal(Task&, std::vector<Task>);
    ~Scheduler() = default;
};

class LocalScheduler : public Scheduler {
public:
    double scheduleTask(Task&, std::vector<Task>);
};

class CloudScheduler : public Scheduler {
public:
    double scheduleTask(Task&, std::vector<Task>, std::vector<Task>, double, double, double);
    double calculatereadyTimeWirelessSending(Task&, std::vector<Task>);
    double calculatereadyTimeWirelessComputation(Task&, std::vector<Task>);
};

class LocalSchedulerAssignedCore : public Scheduler
{
    public:
    void scheduleTask(Task& , std::vector<Task>&, std::vector<Task>&);
};

#endif
