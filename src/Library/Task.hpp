#ifndef TASK_HPP
#define TASK_HPP

#include <vector>

using std::vector;

class Task
{
private:
    int m_taskNumber;
    bool m_isEntry;
    bool m_isExit;
    int m_inDegreeEdges;
    int m_outDegreeEdges;
    vector<double> m_taskCoreValues;
    bool m_isCloudTask;
    double m_computationCost;
    double m_priority;
    double m_taskStartTime;
    double m_taskFinishTime;
    int m_channelNumber;
    int ready1;
    bool ready2;
    vector<int> m_pred;
    vector<int> m_succ;
    vector<int> m_transpred;
    vector<int> m_transsucc;
    double finishTimeLocal;
    double readyTimeLocal;
    double readyTimeWirelessSending;
    double readyTimeWirelessComputation;
    double readyTimeWirelessReceiving;
    double finishTimeWirelessSending;
    double finishTimeWirelessComputation;
    double finishTimeWirelessReceiving;
    double taskEneryConsumption;
    double taskTimeConsumption;
    bool isScheduled;

public:
    // Constructor
    explicit Task(int number);

    // Accessors
    int getTaskNumber() const;
    bool getisEntry() const;
    bool getisExit() const;
    int getInDegreeEdges() const;
    int getOutDegreeEdges() const;
    vector<double> getTaskCoreValues() const;
    bool getisCloudTask() const;
    double getComputationCost() const;
    double getPriority() const;
    vector<int> getPredecessors() const;
    vector<int> getSucessors() const;
    vector<int> getTransistiveSuccessor() const;
    vector<int> getTransistivePredessor() const;
    int getChannelNumber() const;
    double getTaskStartTime() const;
    double getTaskFinishTime() const;
    double getfinishTimeWirelessSending() const;
    double getfinishTimeComputation() const;
    double getfinishTimeWirelessReceiving() const;
    double getreadyTimeWirelessSending() const;
    double getreadyTimeWirelessComputation() const;
    double getreadyTimeWirelessReceiving() const;
    double getfinishTimeLocal() const;
    double getreadyTimeLocal() const;
    double getTaskEnergyConsumption() const;
    double getTaskTimeConsumption() const;
    int getReady1() const;
    bool getReady2() const;
    bool getIsScheduled() const;

    // Mutators
    void setEntry(bool input);
    void setExit(bool input);
    void incrementInDegree();
    void incrementOutDegree();
    void setTaskCoreValues(double input);
    void setisCloudTask(bool input);
    void setComputationCost(double input);
    void setPriority(double input);
    void setPredessor(int input);
    void setSuccessor(int input);
    void updatePredessor(vector<int> input);
    void setTransistiveSuccessor(int input);
    void setTransistivePredessor(int input);
    void setChannelNumber(int input);
    void setTaskStartTime(double input);
    void setTaskFinishTime(double input);
    void setfinishTimeWirelessSending(double input);
    void setfinishTimeWirelessComputation(double input);
    void setfinishTimeWirelessReceiving(double input);
    void setreadyTimeWirelessSending(double input);
    void setreadyTimeWirelessComputation(double input);
    void setreadyTimeWirelessReceiving(double input);
    void setFinishTimeLocal(double input);
    void setReadyTimeLocal(double input);
    void setTaskEnergyConsumption(double input);
    void setTaskTimeConsumption(double input);
    void setReady1(int input);
    void setReady2(bool input);
    void setIsScheduled(bool input);
    void clearTransPredSucc();
    void clearPredecessors();
    void clearSuccessors();
    void clearTaskCoreValues();
};

#endif // TASK_HPP
