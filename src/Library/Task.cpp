#include "Task.hpp"

Task::Task(int number)
    : m_taskNumber(number),
      m_isEntry(true),
      m_isExit(true),
      m_inDegreeEdges(0),
      m_outDegreeEdges(0),
      m_taskCoreValues(0),
      m_isCloudTask(true),
      m_computationCost(0),
      m_pred(0),
      m_succ(0) {}

// Accessors
int Task::getTaskNumber() const { return m_taskNumber; }
bool Task::getisEntry() const { return m_isEntry; }
bool Task::getisExit() const { return m_isExit; }
int Task::getInDegreeEdges() const { return m_inDegreeEdges; }
int Task::getOutDegreeEdges() const { return m_outDegreeEdges; }
vector<double> Task::getTaskCoreValues() const { return m_taskCoreValues; }
bool Task::getisCloudTask() const { return m_isCloudTask; }
double Task::getComputationCost() const { return m_computationCost; }
double Task::getPriority() const { return m_priority; }
vector<int> Task::getPredecessors() const { return m_pred; }
vector<int> Task::getSucessors() const { return m_succ; }
vector<int> Task::getTransistiveSuccessor() const { return m_transsucc; }
vector<int> Task::getTransistivePredessor() const { return m_transpred; }
int Task::getChannelNumber() const { return m_channelNumber; }
double Task::getTaskStartTime() const { return m_taskStartTime; }
double Task::getTaskFinishTime() const { return m_taskFinishTime; }
double Task::getfinishTimeWirelessSending() const { return finishTimeWirelessSending; }
double Task::getfinishTimeComputation() const { return finishTimeWirelessComputation; }
double Task::getfinishTimeWirelessReceiving() const { return finishTimeWirelessReceiving; }
double Task::getreadyTimeWirelessSending() const { return readyTimeWirelessSending; }
double Task::getreadyTimeWirelessComputation() const { return readyTimeWirelessComputation; }
double Task::getreadyTimeWirelessReceiving() const { return readyTimeWirelessReceiving; }
double Task::getfinishTimeLocal() const { return finishTimeLocal; }
double Task::getreadyTimeLocal() const { return readyTimeLocal; }
double Task::getTaskEnergyConsumption() const { return taskEneryConsumption; }
double Task::getTaskTimeConsumption() const { return taskTimeConsumption; }
int Task::getReady1() const { return ready1; }
bool Task::getReady2() const { return ready2; }
bool Task::getIsScheduled() const { return isScheduled; }

// Mutators
void Task::setEntry(bool input) { m_isEntry = input; }
void Task::setExit(bool input) { m_isExit = input; }
void Task::incrementInDegree() { ++m_inDegreeEdges; }
void Task::incrementOutDegree() { ++m_outDegreeEdges; }
void Task::setTaskCoreValues(double input) { m_taskCoreValues.push_back(input); }
void Task::setisCloudTask(bool input) { m_isCloudTask = input; }
void Task::setComputationCost(double input) { m_computationCost = input; }
void Task::setPriority(double input) { m_priority = input; }
void Task::setPredessor(int input) { m_pred.push_back(input); }
void Task::setSuccessor(int input) { m_succ.push_back(input); }
void Task::updatePredessor(vector<int> input) { m_pred = input; }
void Task::setTransistiveSuccessor(int input) { m_transsucc.push_back(input); }
void Task::setTransistivePredessor(int input) { m_transpred.push_back(input); }
void Task::setChannelNumber(int input) { m_channelNumber = input; }
void Task::setTaskStartTime(double input) { m_taskStartTime = input; }
void Task::setTaskFinishTime(double input) { m_taskFinishTime = input; }
void Task::setfinishTimeWirelessSending(double input) { finishTimeWirelessSending = input; }
void Task::setfinishTimeWirelessComputation(double input) { finishTimeWirelessComputation = input; }
void Task::setfinishTimeWirelessReceiving(double input) { finishTimeWirelessReceiving = input; }
void Task::setreadyTimeWirelessSending(double input) { readyTimeWirelessSending = input; }
void Task::setreadyTimeWirelessComputation(double input) { readyTimeWirelessComputation = input; }
void Task::setreadyTimeWirelessReceiving(double input) { readyTimeWirelessReceiving = input; }
void Task::setFinishTimeLocal(double input) { finishTimeLocal = input; }
void Task::setReadyTimeLocal(double input) { readyTimeLocal = input; }
void Task::setTaskEnergyConsumption(double input) { taskEneryConsumption = input; }
void Task::setTaskTimeConsumption(double input) { taskTimeConsumption = input; }
void Task::setReady1(int input) { ready1 = input; }
void Task::setReady2(bool input) { ready2 = input; }
void Task::setIsScheduled(bool input) { isScheduled = input; }
void Task::clearTransPredSucc() { m_transpred.clear(); m_transsucc.clear(); }
void Task::clearPredecessors() { m_pred.clear(); }
void Task::clearSuccessors() { m_succ.clear(); }
void Task::clearTaskCoreValues() { m_taskCoreValues.clear(); }
