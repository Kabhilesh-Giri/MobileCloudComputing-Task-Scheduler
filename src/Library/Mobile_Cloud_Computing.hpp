#ifndef MOBILE_CLOUD_COMPUTING_HPP
#define MOBILE_CLOUD_COMPUTING_HPP

#include <iostream>
#include "config.hpp"
#include "Task.hpp"
#include "Task_Graph.hpp"
#include "Scheduler.hpp"
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <array>
#include <stack>

constexpr int CLOUD_TASK = 3;
static constexpr int NUM_CHANNELS = 4;

class TaskGraph;
class Task;
class LocalScheduler;
class CloudScheduler;
class LocalSchedulerAssignedCore;

class Mobile_Cloud_Computing
{
private:
    TaskGraph taskGraph;
    std::vector<std::vector<double>> m_coreValues;
    std::vector<std::pair<int, int>> m_taskGraph;
    double m_timeSending;
    double m_timeComputation;
    double m_timeReceiving;
    double m_powerSending;
    double m_powerCoreOneComputataion;
    double m_powerCoreTwoComputataion;
    double m_powerCoreThreeComputataion;
    std::vector<Task> m_Tasks;
    int m_N;
    std::vector<Task> m_TaskPriotizingScheduledTasks;
    std::vector<Task> m_StepOneScheduledTasks;
    std::vector<Task> m_StepTwoScheduledTasks;

    //Phase One : Energy and Time Consumption Phase
    double m_StepOneTotalApplicationCompletionTime{};
    double m_StepOneTotalApplicationEnergy{};


    //Auxilliary Functions
    int findTaskWithHighestPriorityValue(std::vector<Task>) const;
    void updateTaskSequences(std::vector<Task>&, std::vector<Task>&, std::vector<Task>&, std::vector<Task>&, std::vector<Task>&);
    void copyTaskValues(std::vector<Task>& , const std::vector<Task>&);
    void removeTaskFromSequence(std::vector<Task>&, int);
    void addAndSortSequence(std::vector<Task>&, const Task&, const std::vector<Task>&);
    void updateReady1AndReady2(const std::vector<Task>&, std::vector<Task>&, const std::array<std::vector<Task>, 4>&);

    //Step One Functions
    void primaryAssignment();
    void taskPrioritizing();
    void executionUnitSelection();

    //Step Two Functions
    void outerloop();
    void TaskMigration(std::vector<Task>&, double);
    void kernel_algorithm(std::vector<Task>, std::vector<Task>&, int, Task&, const std::vector<Task>&, const std::vector<Task>&, const std::vector<Task>&, const std::vector<Task>&);

    //Energy And Time Calculation
    void EnergyConsumptionAndApplicationCompletionTime(std::vector<Task>&, double&, double&);

public:
    Mobile_Cloud_Computing(const std::vector<std::vector<double>>& , const std::vector<std::pair<int, int>>& , double , double , double ,double , double , double, double);
    void runStepOneScheduling();
    void runStepTwoScheduling();
    void displayResults();
    ~Mobile_Cloud_Computing() = default;
};

#endif // MOBILE_CLOUD_COMPUTING_HPP
