#include "Scheduler.hpp"

double Scheduler::findMaxValue(const std::vector<double>& vect) const
{
    if (vect.empty()) {
        throw std::runtime_error("Vector is empty, cannot find maximum value.");
    }
    return *std::max_element(vect.begin(), vect.end());
}

double Scheduler::calculatereadyTimeLocal(Task& task, std::vector<Task> OrderedScheduledTasks)
{
    if (!OrderedScheduledTasks.empty())
    {
        std::vector<double> MaxElements; // Correct initialization
        std::vector<int> ImmediatePredecessors = task.getPredecessors();

        for (int pred : ImmediatePredecessors) {
            auto it = std::find_if(OrderedScheduledTasks.begin(), OrderedScheduledTasks.end(),
            [pred](const Task& t) { return t.getTaskNumber() == pred + 1; });

            if (it != OrderedScheduledTasks.end())
            {
                double predFinishTime = it->getisCloudTask() ? it->getfinishTimeWirelessReceiving() : it->getfinishTimeLocal();
                MaxElements.push_back(predFinishTime);
            }
            else
            {
                throw std::runtime_error("Immediate predecessor task not found in scheduled tasks.");
            }
        }
        if (MaxElements.empty())
        {
            return 0;
        }

        return findMaxValue(MaxElements);
    }

    return 0;
}

double CloudScheduler::calculatereadyTimeWirelessSending(Task& task, std::vector<Task> m_StepOneScheduledTasks)
{
    if(m_StepOneScheduledTasks.size() != 0)
    {
        std::vector<double> MaxElements;
        std::vector<int> ImmediatePredeccesors = task.getPredecessors();
        for(auto pred : ImmediatePredeccesors)
        {
            auto it = std::find_if(m_StepOneScheduledTasks.begin(),m_StepOneScheduledTasks.end(),[pred]
            (const Task &t) {return t.getTaskNumber() == pred + 1;});

            if(it != m_StepOneScheduledTasks.end())
            {
                if(it->getisCloudTask() == 1)
                {
                    MaxElements.push_back(it->getfinishTimeWirelessSending());
                }
                else
                {
                    MaxElements.push_back(it->getfinishTimeLocal());
                }
            }
        }
        if (MaxElements.empty())
        {
            return 0;
        }
        return findMaxValue(MaxElements);
        }
    return 0;
}

double CloudScheduler::calculatereadyTimeWirelessComputation(Task& task, std::vector<Task> m_StepOneScheduled)
{
    std::vector<double> MaxElements;
    std::vector<int> ImmediatePredeccesors = task.getPredecessors();

    for(auto pred : ImmediatePredeccesors)
    {
        auto it = std::find_if(m_StepOneScheduled.begin(),m_StepOneScheduled.end(),[pred](const Task& t){
            return t.getTaskNumber() == pred + 1;
        });

        if (it != m_StepOneScheduled.end())
        {
            //No issue if it local or cloud as we are going to calculate among finishtimecomputation and finishtimewirelesssending
            MaxElements.push_back(it->getfinishTimeComputation());
        }
    }

    auto value = findMaxValue(MaxElements);

    return (value > task.getfinishTimeWirelessSending()) ? value : task.getfinishTimeWirelessSending();
}

//Passing the m_StepOneScheduledTasks as pass by value making sure the changes made in other functionality are not affecting
double CloudScheduler::scheduleTask(Task& task, std::vector<Task> m_NewScheduledTasks, std::vector<Task> m_StepOneScheduled, double timeSending, double timeComputation, double timeReceiving)
{
    double maxTempPredecessorSendingTime {};
    double maxTempPredecessorComputationTime {};
    double maxTempPredecessorReceivingTime {};

    // Sending Phase

    //Find ReadyTimeWirelessSending on Wireless Channel
    task.setreadyTimeWirelessSending(calculatereadyTimeWirelessSending(task,m_StepOneScheduled));

    //writing base condition
    if(m_NewScheduledTasks.size() == 0)
    {
        task.setfinishTimeWirelessSending(timeSending); //As it is the intital value
        task.setreadyTimeWirelessComputation(timeSending);
        task.setfinishTimeWirelessComputation(timeSending + timeComputation);
        task.setreadyTimeWirelessReceiving(timeSending + timeComputation);
        return (timeSending + timeComputation + timeReceiving);
    }

    //We calculate FT_ws from the schedule, and then the cloud will begin executing task at the ready time RT_c
    // *Note: FinishTime of these tasks are nothing but cloud based tasks so ensure using channel number = 3
    for(auto predecessor : m_NewScheduledTasks)
    {
        if(predecessor.getChannelNumber() == 3 && maxTempPredecessorSendingTime < predecessor.getfinishTimeWirelessSending())
        {
            maxTempPredecessorSendingTime = predecessor.getfinishTimeWirelessSending();
        }
    }
    /* Please note that the mobile device might not be able to start offloading task
    at time RT_ws if it is offloading other tasks at that time */
    task.setfinishTimeWirelessSending(std::max(maxTempPredecessorSendingTime, task.getreadyTimeWirelessSending()) + timeSending);

    // Computation Phase

    task.setreadyTimeWirelessComputation(calculatereadyTimeWirelessComputation(task,m_StepOneScheduled));

    for(auto predecessor : m_NewScheduledTasks)
    {
        if(predecessor.getChannelNumber() == 3 && maxTempPredecessorComputationTime < predecessor.getfinishTimeComputation())
        {
            maxTempPredecessorComputationTime = predecessor.getfinishTimeComputation();
        }
    }

    task.setfinishTimeWirelessComputation((maxTempPredecessorSendingTime > task.getreadyTimeWirelessComputation()) ?
    (maxTempPredecessorComputationTime + timeComputation) : (task.getreadyTimeWirelessComputation() + timeComputation));

    //Receiving Phase

    task.setreadyTimeWirelessReceiving(task.getfinishTimeComputation()); //FT_wr

    for (auto predecessor : m_NewScheduledTasks)
    {
        if (predecessor.getChannelNumber() == 3 && maxTempPredecessorReceivingTime < predecessor.getfinishTimeComputation())
        {
            maxTempPredecessorReceivingTime = predecessor.getfinishTimeComputation();
        }
    }

    task.setfinishTimeWirelessReceiving( (maxTempPredecessorReceivingTime > task.getfinishTimeComputation()) ?
    (maxTempPredecessorReceivingTime + timeReceiving) : (task.getfinishTimeComputation() + timeReceiving));

    return task.getfinishTimeWirelessReceiving();
}

double LocalScheduler::scheduleTask(Task& task, std::vector<Task> OrderedScheduledTasks)
{
    task.setReadyTimeLocal(calculatereadyTimeLocal(task,OrderedScheduledTasks));
    double tempEarliestFinishTimeLocal {std::numeric_limits<double>::max()};
    double tempMaxFinishTimeCheckAllCores{};
    double tempMaxFinishTimeIfDelayExist{};
    std::vector<double> tempVectTaskCoreValues {task.getTaskCoreValues()};

    // writing base condition
    if(OrderedScheduledTasks.size() == 0)
    {
        for(size_t i{}; i<task.getTaskCoreValues().size(); i++)
        {
            if(tempEarliestFinishTimeLocal > tempVectTaskCoreValues[i])
            {
                tempEarliestFinishTimeLocal = tempVectTaskCoreValues[i];
                task.setChannelNumber(i);
            }
        }
        return tempEarliestFinishTimeLocal;
    }

    // Iterate through available cores to find the earliest finish time
    for (size_t i{}; i < tempVectTaskCoreValues.size(); i++) {
        tempMaxFinishTimeIfDelayExist = task.getreadyTimeLocal() + tempVectTaskCoreValues[i];
        tempMaxFinishTimeCheckAllCores = 0;

        // Check for the latest finish time of tasks on the same core
        for (size_t j = 0; j < OrderedScheduledTasks.size(); j++) {
            if ((OrderedScheduledTasks[j].getChannelNumber() == static_cast<int>(i)) && (tempMaxFinishTimeCheckAllCores < OrderedScheduledTasks[j].getfinishTimeLocal())) {
                tempMaxFinishTimeCheckAllCores = OrderedScheduledTasks[j].getfinishTimeLocal();
            }
        }

        // Update the finish time if there's a delay
        if (tempMaxFinishTimeCheckAllCores > task.getreadyTimeLocal()) {
            tempMaxFinishTimeIfDelayExist = tempMaxFinishTimeCheckAllCores + tempVectTaskCoreValues[i];
        }

        // Update the earliest finish time and core assignment
        if (tempEarliestFinishTimeLocal > tempMaxFinishTimeIfDelayExist) {
            tempEarliestFinishTimeLocal = tempMaxFinishTimeIfDelayExist;
            task.setChannelNumber(i);
        }
    }
    return tempEarliestFinishTimeLocal; // Return the earliest finish time
}

void LocalSchedulerAssignedCore::scheduleTask(Task& SelectedTask, std::vector<Task>& NewScheduledTasks, std::vector<Task>& m_StepOneScheduled)
{
    double tempStartTime{};
    SelectedTask.setReadyTimeLocal(calculatereadyTimeLocal(SelectedTask,m_StepOneScheduled));
    SelectedTask.setTaskStartTime(SelectedTask.getreadyTimeLocal());
    std::vector<double> tempVectTaskCoreValues {SelectedTask.getTaskCoreValues()};
    if(NewScheduledTasks.size() == 0)
    {
        SelectedTask.setFinishTimeLocal(SelectedTask.getreadyTimeLocal() + tempVectTaskCoreValues[SelectedTask.getChannelNumber()]);
    }
    else
    {
        int size = static_cast<int>(NewScheduledTasks.size());
        for(int i{}; i < size ; i++)
        {
            if((NewScheduledTasks[i].getChannelNumber() == SelectedTask.getChannelNumber()) && (tempStartTime < NewScheduledTasks[i].getfinishTimeLocal()))
            {
                tempStartTime = NewScheduledTasks[i].getfinishTimeLocal();
            }
        }
        if(tempStartTime > SelectedTask.getreadyTimeLocal())
        {
            SelectedTask.setFinishTimeLocal(tempStartTime + tempVectTaskCoreValues[SelectedTask.getChannelNumber()]);
            SelectedTask.setReadyTimeLocal(tempStartTime);
        }
        else{
            SelectedTask.setFinishTimeLocal(SelectedTask.getreadyTimeLocal() + tempVectTaskCoreValues[SelectedTask.getChannelNumber()]);
            SelectedTask.setReadyTimeLocal(SelectedTask.getreadyTimeLocal());
        }
    }
    SelectedTask.setTaskFinishTime(SelectedTask.getfinishTimeLocal());
    SelectedTask.setTaskStartTime(SelectedTask.getreadyTimeLocal());
}
