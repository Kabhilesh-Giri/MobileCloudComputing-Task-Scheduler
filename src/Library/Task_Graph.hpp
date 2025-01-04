#ifndef TASK_GRAPH_HPP
#define TASK_GRAPH_HPP

#include <vector>
#include <unordered_map>
#include <set>
#include <stdexcept>

#include "Task.hpp"

class Task;

class TaskGraph {
private:
    std::vector<Task> m_Tasks; // List of tasks in the graph
    const std::vector<std::pair<int, int>>& m_taskGraph; // Edges representing task dependencies
    int m_taskGraphSize = 0; // Total number of unique tasks
    const std::vector<std::vector<double>>& m_coreValues; // Core values for each task

    void findSizeOfGraph();
    void initializeTasks();
    void processEdges();
    void identifyEntryAndExitTasks();
    void identifyEachTaskCoreValues();
    void initialize(); // Consolidated initialization logic

public:
    // Constructor
    TaskGraph(const std::vector<std::vector<double>>&, const std::vector<std::pair<int, int>>&);

    // Accessors
    std::vector<Task> getTasks();
    Task& getTaskById(int id);
    int getTaskGraphSize() const { return m_taskGraphSize; }
};

#endif // TASK_GRAPH_HPP
