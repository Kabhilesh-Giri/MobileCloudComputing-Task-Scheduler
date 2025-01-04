#include "Task_Graph.hpp"

TaskGraph::TaskGraph(const std::vector<std::vector<double>>& coreValues,
const std::vector<std::pair<int, int>>& edges) : m_taskGraph(edges) , m_coreValues(coreValues)
{
    initialize();
}

void TaskGraph::initialize() {

    findSizeOfGraph();
    initializeTasks();
    processEdges();
    identifyEntryAndExitTasks();
    identifyEachTaskCoreValues();
}

std::vector<Task> TaskGraph::getTasks()
{
    return m_Tasks;
}

void TaskGraph::findSizeOfGraph() {
    // Use a set to find the unique task IDs
    std::set<int> uniqueValues;
    for (const auto& edge : m_taskGraph) {
        uniqueValues.insert(edge.first);
        uniqueValues.insert(edge.second);
    }
    m_taskGraphSize = uniqueValues.size();
}

void TaskGraph::initializeTasks() {
    // Initialize tasks based on the number of unique task IDs
    for (int i = 1; i <= m_taskGraphSize; ++i) {
        m_Tasks.emplace_back(Task(i));
    }
}

void TaskGraph::processEdges() {
    // Initialize immediate successors and predecessors based on edges
    for (const auto& edge : m_taskGraph) {
        int from = edge.first;
        int to = edge.second;

        if (from <= 0 || to <= 0 || from > m_taskGraphSize || to > m_taskGraphSize) {
            throw std::out_of_range("Invalid edge indices");
        }

        m_Tasks[from - 1].incrementOutDegree();
        m_Tasks[to - 1].incrementInDegree();
        m_Tasks[from - 1].setSuccessor(to - 1); // Store successor (0-based indexing)
        m_Tasks[to - 1].setPredessor(from - 1); // Store predecessor (0-based indexing)
    }
}

void TaskGraph::identifyEntryAndExitTasks() {
    // Determine if a task is an entry or exit task
    for (auto& task : m_Tasks) {
        task.setEntry(task.getInDegreeEdges() == 0);
        task.setExit(task.getOutDegreeEdges() == 0);
    }
}

void TaskGraph::identifyEachTaskCoreValues() {

    if (m_coreValues.size() != m_Tasks.size()) {
        throw std::invalid_argument("Mismatch between tasks and core values");
    }

    // Assign core execution times to tasks
    for (size_t i = 0; i < m_Tasks.size(); ++i) {
        for (double value : m_coreValues[i]) {
            m_Tasks[i].setTaskCoreValues(value);
        }
    }
}
