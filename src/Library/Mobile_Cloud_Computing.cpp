#include "Mobile_Cloud_Computing.hpp"

Mobile_Cloud_Computing::Mobile_Cloud_Computing(const std::vector<std::vector<double>>& coreValues, const std::vector<std::pair<int, int>>& DAG,
double timeSending, double timeComputation, double timeReceiving, double powerSending, double powerCoreOneComputataion, double powerCoreTwoComputataion,
double powerCoreThreeComputataion) : taskGraph(coreValues,DAG), m_coreValues(coreValues), m_taskGraph(DAG), m_timeSending(timeSending), m_timeComputation(timeComputation),
m_timeReceiving(timeReceiving) , m_powerSending(powerSending), m_powerCoreOneComputataion(powerCoreOneComputataion), m_powerCoreTwoComputataion(powerCoreTwoComputataion),
m_powerCoreThreeComputataion(powerCoreThreeComputataion), m_Tasks(taskGraph.getTasks()) , m_N(taskGraph.getTaskGraphSize())
{
    // Validate inputs
    if (m_coreValues.empty() || m_taskGraph.empty()) {
        std::cerr << "Error: Task graph or core values are empty." << std::endl;
    }
}

void Mobile_Cloud_Computing::runStepOneScheduling()
{
    primaryAssignment();
    taskPrioritizing();
    executionUnitSelection();
}

void Mobile_Cloud_Computing::runStepTwoScheduling()
{
    outerloop();
}

int Mobile_Cloud_Computing::findTaskWithHighestPriorityValue(std::vector<Task> temp) const
{
    int tasknumber{-1};
    double max = std::numeric_limits<double>::min();
    for(auto task : temp)
    {
        if(max<task.getPriority())
        {
            max = task.getPriority();
            tasknumber = (task.getTaskNumber() - 1); //Deriving Index Based Address
        }
    }
    return tasknumber;
}

void Mobile_Cloud_Computing::primaryAssignment()
{
    double totalCloudTime = m_timeSending + m_timeComputation + m_timeReceiving;
    double min = std::numeric_limits<double>::max();
    for(auto &task : m_Tasks)
    {
        min = std::numeric_limits<double>::max(); // Reset for each task
        auto value = task.getTaskCoreValues();
        for(size_t i{};i<task.getTaskCoreValues().size();i++)
            {if(min>value[i]) {min = value[i];}}
        if(min>totalCloudTime)
        {
            task.setisCloudTask(true);
        }
        else{
            task.setisCloudTask(false);
        }
    }
}


void Mobile_Cloud_Computing::taskPrioritizing()
{
    double sum{};
    m_StepOneScheduledTasks.clear(); //Making sure it doesn't carry any garbage values

    // Calculate Computation Cost for each Task
    for(auto &task : m_Tasks)
    {
        std::vector<double> vect = task.getTaskCoreValues();
        if(task.getisCloudTask())
        {task.setComputationCost(m_timeSending + m_timeComputation + m_timeReceiving);}

        else if (!task.getisCloudTask())
        {
            for(size_t i{}; i<task.getTaskCoreValues().size();i++)
            {sum+= vect[i];}
            task.setComputationCost(sum/task.getTaskCoreValues().size());
            sum = 0;
        }
    }

    // Calculate Priority Values using this formula
    // Priority(task) = ComputationCost(task) + max(Priority(sucessors))
    // Exit Task -> Priority(task) = ComputationCost(task)

    for(auto &task : m_Tasks)
    {
        if(task.getisExit() == 1)
        {
            task.setPriority(task.getComputationCost());
        }
    }

    std::vector<int> value;

    for (int i = m_N - 1; i >= 0; --i) {
    double maxPriority = 0;
    for (int succ : m_Tasks[i].getSucessors()) {
        maxPriority = std::max(maxPriority, m_Tasks[succ].getPriority());
    }
    m_Tasks[i].setPriority(m_Tasks[i].getComputationCost() + maxPriority);}
}

void Mobile_Cloud_Computing::executionUnitSelection()
{
    std::vector<Task> temp = m_Tasks; // Local copy of m_Tasks
    double tempTimeLocalScheduling{};
    double tempTimeCloudScheduling{};
    int temptasknum{-1};
    LocalScheduler localScheduler;
    CloudScheduler cloudScheduler;

    while (!temp.empty())
    {
        // Find the task number with the highest priority
        temptasknum = findTaskWithHighestPriorityValue(temp);

        /*The std::find_if function from the Standard Template Library (STL) returns an iterator to the first element
        in the range [first, last) that satisfies the given predicate (condition). If no such element is found, it
        returns last, which usually corresponds to container.end().*/

        auto it = std::find_if(temp.begin(), temp.end(), [temptasknum](const Task& task) {
            return task.getTaskNumber() == temptasknum + 1; // Convert 0-based index to 1-based task number
        });

        if (it != temp.end())
        {
            // Add the task to the scheduled tasks
            m_TaskPriotizingScheduledTasks.emplace_back(*it);

            // Erase the task from the temp std::vector
            temp.erase(it);
        }
        else
        {
            throw std::runtime_error("Task with the given number not found in temp");
        }
    }

    for(auto &task : m_TaskPriotizingScheduledTasks)
    {
        if(task.getisCloudTask() == 1)
        {
            task.setFinishTimeLocal(0);
            task.setReadyTimeLocal(0);
            task.setChannelNumber(CLOUD_TASK);
            task.setfinishTimeWirelessReceiving(cloudScheduler.scheduleTask(task, m_StepOneScheduledTasks, m_TaskPriotizingScheduledTasks, m_timeSending, m_timeComputation, m_timeReceiving));
            task.setTaskStartTime(task.getfinishTimeWirelessReceiving() - (m_timeSending + m_timeComputation + m_timeReceiving));
            task.setTaskFinishTime(task.getfinishTimeWirelessReceiving());
        }
        else
        {
            /*If the selected task is not a cloud task, it may be scheduled on a local core or the cloud. We need to estimate the
            finish time of this task if it is scheduled on each core and the finish time of this task if it is offloaded to the cloud,*/
            task.setfinishTimeWirelessSending(0);
            task.setfinishTimeWirelessComputation(0);
            task.setfinishTimeWirelessReceiving(0);
            task.setreadyTimeWirelessSending(0);
            task.setreadyTimeWirelessComputation(0);
            task.setreadyTimeWirelessReceiving(0);

            tempTimeLocalScheduling = localScheduler.scheduleTask(task, m_TaskPriotizingScheduledTasks);
            tempTimeCloudScheduling = cloudScheduler.scheduleTask(task, m_StepOneScheduledTasks, m_TaskPriotizingScheduledTasks, m_timeSending, m_timeComputation, m_timeReceiving);

            if (tempTimeCloudScheduling < tempTimeLocalScheduling)
            {
                // Schedule task on the cloud
                task.setReadyTimeLocal(0);
                task.setFinishTimeLocal(0);
                task.setisCloudTask(true);
                task.setChannelNumber(CLOUD_TASK);
                task.setfinishTimeWirelessReceiving(tempTimeCloudScheduling);
                task.setTaskStartTime(task.getfinishTimeWirelessReceiving() - (m_timeSending + m_timeComputation + m_timeReceiving));
                task.setTaskFinishTime(task.getfinishTimeWirelessReceiving());
            }

            else {
                // Schedule task on a local core
                task.setisCloudTask(false);
                task.setfinishTimeWirelessSending(0);
                task.setfinishTimeWirelessComputation(0);
                task.setfinishTimeWirelessReceiving(0);
                task.setreadyTimeWirelessSending(0);
                task.setreadyTimeWirelessComputation(0);
                task.setreadyTimeWirelessReceiving(0);
                task.setFinishTimeLocal(tempTimeLocalScheduling);
                task.setTaskStartTime(task.getfinishTimeLocal() - task.getTaskCoreValues()[task.getChannelNumber()]);
                task.setTaskFinishTime(task.getfinishTimeLocal());
            }
        }
        m_StepOneScheduledTasks.push_back(task);
    }
}

void Mobile_Cloud_Computing::removeTaskFromSequence(std::vector<Task>& sequence, int elementToRemove) {
    sequence.erase(
        std::remove_if(sequence.begin(), sequence.end(),[&elementToRemove](const Task& t) {
            return t.getTaskNumber() == elementToRemove;}),
        sequence.end());
}

void Mobile_Cloud_Computing::addAndSortSequence(std::vector<Task>& sequence, const Task& task,
                        const std::vector<Task>& m_StepOneScheduled) {
    sequence.push_back(task);
    auto comparator = [&m_StepOneScheduled](const Task& t1, const Task& t2) {
        auto it1 = std::find_if(m_StepOneScheduled.begin(), m_StepOneScheduled.end(),
                                [&t1](const Task& t) {
                                    return t.getTaskNumber() == t1.getTaskNumber();
                                });
        auto it2 = std::find_if(m_StepOneScheduled.begin(), m_StepOneScheduled.end(),
                                [&t2](const Task& t) {
                                    return t.getTaskNumber() == t2.getTaskNumber();
                                });

        if (it1 == m_StepOneScheduled.end() || it2 == m_StepOneScheduled.end()) {
            throw std::runtime_error("Task not found in m_StepOneScheduled");
        }

        return it1->getTaskStartTime() <= it2->getTaskStartTime();
    };
    std::sort(sequence.begin(), sequence.end(), comparator);
}

void Mobile_Cloud_Computing::copyTaskValues(std::vector<Task>& m_StepOneScheduledTasks, const std::vector<Task>& NewScheduledTasks) {
    // Create a mapping of task numbers to their corresponding objects in NewScheduledTasks
    std::unordered_map<int, const Task*> taskMap;
    for (const auto& task : NewScheduledTasks) {
        taskMap[task.getTaskNumber()] = &task;
    }

    // Update values in m_StepOneScheduledTasks using the mapping
    for (auto& task : m_StepOneScheduledTasks) {
        int taskNumber = task.getTaskNumber();
        if (taskMap.find(taskNumber) != taskMap.end()) {
            const Task* newTask = taskMap[taskNumber];

            // Update basic fields
            task.setEntry(newTask->getisEntry());
            task.setExit(newTask->getisExit());
            task.setisCloudTask(newTask->getisCloudTask());
            task.setComputationCost(newTask->getComputationCost());
            task.setPriority(newTask->getPriority());

            // Update core values
            task.clearTaskCoreValues(); // Assuming a `clearTaskCoreValues` method exists
            for (const auto& coreValue : newTask->getTaskCoreValues()) {
                task.setTaskCoreValues(coreValue);
            }

            // Update predecessors and successors
            task.clearPredecessors(); // Assuming a `clearPredecessors` method exists
            for (const auto& pred : newTask->getPredecessors()) {
                task.setPredessor(pred);
            }

            task.clearSuccessors(); // Assuming a `clearSuccessors` method exists
            for (const auto& succ : newTask->getSucessors()) {
                task.setSuccessor(succ);
            }

            // Update channel and timing values
            task.setChannelNumber(newTask->getChannelNumber());
            task.setTaskStartTime(newTask->getTaskStartTime());
            task.setTaskFinishTime(newTask->getTaskFinishTime());
            task.setfinishTimeWirelessSending(newTask->getfinishTimeWirelessSending());
            task.setfinishTimeWirelessComputation(newTask->getfinishTimeComputation());
            task.setfinishTimeWirelessReceiving(newTask->getfinishTimeWirelessReceiving());
            task.setreadyTimeWirelessSending(newTask->getreadyTimeWirelessSending());
            task.setreadyTimeWirelessComputation(newTask->getreadyTimeWirelessComputation());
            task.setreadyTimeWirelessReceiving(newTask->getreadyTimeWirelessReceiving());
            task.setFinishTimeLocal(newTask->getfinishTimeLocal());
            task.setReadyTimeLocal(newTask->getreadyTimeLocal());

            // Update energy and time consumption
            task.setTaskEnergyConsumption(newTask->getTaskEnergyConsumption());
            task.setTaskTimeConsumption(newTask->getTaskTimeConsumption());

            // Update ready and scheduled status
            task.setReady1(newTask->getReady1());
            task.setReady2(newTask->getReady2());
            task.setIsScheduled(newTask->getIsScheduled());
        } else {
            std::cerr << "Warning: Task " << taskNumber << " not found in NewScheduledTasks.\n";
        }
    }
}

void Mobile_Cloud_Computing::updateTaskSequences(std::vector<Task>& Sorg_1,std::vector<Task>& Sorg_2,std::vector<Task>& Sorg_3,
std::vector<Task>& Sorg_C,std::vector<Task>& m_StepOneScheduledTasksCopy)
{
    Sorg_1.clear();
    Sorg_2.clear();
    Sorg_3.clear();
    Sorg_C.clear();

    int size = static_cast<int>(m_StepOneScheduledTasksCopy.size());
    for(int i{}; i<size; i++)
    {
        int channel = m_StepOneScheduledTasksCopy[i].getChannelNumber();
        switch (channel)
        {
        case 0: //Core 1
            Sorg_1.emplace_back(m_StepOneScheduledTasksCopy[i]);
            break;

        case 1: //Core 2
            Sorg_2.emplace_back(m_StepOneScheduledTasksCopy[i]);
            break;

        case 2: //Core 3
            Sorg_3.emplace_back(m_StepOneScheduledTasksCopy[i]);
            break;

        case 3: //Cloud
            Sorg_C.emplace_back(m_StepOneScheduledTasksCopy[i]);
            break;

        default:
            break;
        }
    }
}

void Mobile_Cloud_Computing::updateReady1AndReady2(const std::vector<Task>& NewScheduledTasks,
                           std::vector<Task>& m_StepOneScheduled,
                           const std::array<std::vector<Task>, NUM_CHANNELS>& Snew) {

    // Convert NewScheduledTasks into a set for fast lookup
    std::unordered_set<int> scheduledTaskNumbers;
    for (const auto& task : NewScheduledTasks) {
        scheduledTaskNumbers.insert(task.getTaskNumber());
    }

    // Update ready1 for each task in m_StepOneScheduled
    for (auto& task : m_StepOneScheduled) {
        if (task.getIsScheduled()) {
            task.setReady1(0); // Scheduled tasks have no unscheduled predecessors
            continue;
        }

        int remainingPredecessors = 0;
        for (int predTaskNumber : task.getPredecessors()) {
            // Check if the predecessor task is not yet scheduled
            if ((predTaskNumber) != 0 && scheduledTaskNumbers.find(predTaskNumber+1) == scheduledTaskNumbers.end()) {
                remainingPredecessors++;
            }
        }
        task.setReady1(remainingPredecessors);
    }

    // Update ready2 for tasks in sequences
    for (const auto& sequence : Snew) {
        for (size_t i = 0; i < sequence.size(); ++i) {
            auto it = std::find_if(
                m_StepOneScheduled.begin(), m_StepOneScheduled.end(),
                [&sequence, i](const Task& t) {
                    return t.getTaskNumber() == sequence[i].getTaskNumber();
                });

            if (it != m_StepOneScheduled.end() && !it->getIsScheduled()) {
                // If it is the first task in the sequence, ready2 = 0
                if (i == 0) {
                    it->setReady2(0);
                } else {
                    // Check if the previous task in the sequence has been scheduled
                    int prevTaskNumber = sequence[i - 1].getTaskNumber();
                    if (scheduledTaskNumbers.find(prevTaskNumber) != scheduledTaskNumbers.end()) {
                        it->setReady2(0);
                    } else {
                        it->setReady2(1);
                    }
                }
            }
        }
    }
}

void Mobile_Cloud_Computing::kernel_algorithm(std::vector<Task> m_StepOneScheduled, std::vector<Task>& NewScheduledTasks, int K_tar, Task& task,
const std::vector<Task>& Sorg_1, const std::vector<Task>& Sorg_2, const std::vector<Task>& Sorg_3, const std::vector<Task>& Sorg_C)
{
    CloudScheduler cloudScheduler;
    LocalSchedulerAssignedCore localScheduler;

    std::array<std::vector<Task>, NUM_CHANNELS> Snew = {Sorg_1, Sorg_2, Sorg_3, Sorg_C};
    int elementToRemove = task.getTaskNumber();

    task.setIsScheduled(false);

    //reset the tasks
    for(auto& task : m_StepOneScheduled)
    {task.setIsScheduled(false);}

    // Remove task from original channel
    for (int i = 0; i < NUM_CHANNELS; i++) {
        if (i == task.getChannelNumber()) {
            removeTaskFromSequence(Snew[i], elementToRemove);
        }
    }

    task.setChannelNumber(K_tar);
    task.setReady1(task.getPredecessors().size());

    auto it = std::find_if(m_StepOneScheduled.begin(),m_StepOneScheduled.end(),[task](const Task& t)
    {return task.getTaskNumber() == t.getTaskNumber();});
    it -> setChannelNumber(K_tar);

    // Add task to target channel and sort
    if (K_tar >= 0 && K_tar < NUM_CHANNELS) {
        addAndSortSequence(Snew[K_tar], task, m_StepOneScheduled);
    }

    // Initialize Ready1 for each task
    for (auto& task : m_StepOneScheduled) {
        task.setReady1(task.getPredecessors().size());
    }

    // Initialize Ready2 for tasks in sequences
    for (auto& sequence : Snew) {
        bool isFirst = true;
        for (const auto& task : sequence) {
            auto it = std::find_if(m_StepOneScheduled.begin(), m_StepOneScheduled.end(),
                                   [&task](const Task& t1) { return task.getTaskNumber() == t1.getTaskNumber(); });
            if (it == m_StepOneScheduled.end()) {
                throw std::runtime_error("Task with number " + task.getTaskNumber());}
            it->setReady2(isFirst ? 0 : 1);
            isFirst = false;
        }
    }

bool Flag = true;
int iterationCount = 0;
const int maxIterations = 1000; // Safety limit
int tempSizeCheck = m_StepOneScheduled.size();

while (Flag && iterationCount++ < maxIterations) {
    std::stack<std::reference_wrapper<Task>> LIFO;

    // Push tasks with ready1 == 0 and ready2 == 0 into the LIFO stack
    for (auto& task : m_StepOneScheduled) {
        if (!task.getIsScheduled() && task.getReady1() == 0 && task.getReady2() == 0) {
            LIFO.push(task);
        }
    }

    // Process tasks in LIFO
    while (!LIFO.empty()) {
        Task& task = LIFO.top().get();
        LIFO.pop();

        // Schedule the task based on its channel
        if (task.getChannelNumber() == 3) {
            task.setisCloudTask(true);
            task.setFinishTimeLocal(0);
            task.setReadyTimeLocal(0);
            task.setfinishTimeWirelessReceiving(cloudScheduler.scheduleTask(task, NewScheduledTasks, m_StepOneScheduled, m_timeSending, m_timeComputation, m_timeReceiving));
            task.setTaskStartTime(task.getfinishTimeWirelessReceiving() - (m_timeSending + m_timeComputation + m_timeReceiving));
            task.setTaskFinishTime(task.getfinishTimeWirelessReceiving());
        } else {
            task.setisCloudTask(false);
            task.setfinishTimeWirelessSending(0);
            task.setfinishTimeWirelessComputation(0);
            task.setfinishTimeWirelessReceiving(0);
            task.setreadyTimeWirelessSending(0);
            task.setreadyTimeWirelessComputation(0);
            task.setreadyTimeWirelessReceiving(0);
            localScheduler.scheduleTask(task, NewScheduledTasks, m_StepOneScheduled);
        }

        task.setIsScheduled(true);
        NewScheduledTasks.push_back(task);

        // Update Ready1 and Ready2 for remaining tasks
        updateReady1AndReady2(NewScheduledTasks, m_StepOneScheduled, Snew);
    }

    // Check if all tasks are scheduled
    if (static_cast<int>(NewScheduledTasks.size()) == tempSizeCheck) {
    Flag = false; // All tasks are scheduled
}
}

    // Throw an error if the safety limit is reached
    if (iterationCount >= maxIterations) {
        throw std::runtime_error("Exceeded maximum iterations in kernel_algorithm");
    }
}

void Mobile_Cloud_Computing::TaskMigration(std::vector<Task>& m_StepOneScheduledTasksCopy, double T_max)
{
    double ActualStepOneApplicationTime {m_StepOneTotalApplicationCompletionTime};
    std::vector<Task> NewScheduledTasks;
    double NewScheduledTasksEnergy{};
    double NewScheduledTasksTime{};
    double BestRatio {0.0};
    EnergyConsumptionAndApplicationCompletionTime(m_StepOneScheduledTasksCopy, m_StepOneTotalApplicationCompletionTime, m_StepOneTotalApplicationEnergy);

    //Create Sequence for Original Scheduling
    std::vector<Task> Sorg_1;
    std::vector<Task> Sorg_2;
    std::vector<Task> Sorg_3;
    std::vector<Task> Sorg_C;

    updateTaskSequences(Sorg_1,Sorg_2,Sorg_3,Sorg_C,m_StepOneScheduledTasksCopy);

    for(auto task : m_StepOneScheduledTasksCopy)
    {
        if(task.getisCloudTask() == 0)
        {
            int currentTaskChannelNum{task.getChannelNumber()};
            for(int i{}; i<NUM_CHANNELS; i++)
            {
                if(i != currentTaskChannelNum)
                {
                    //Calculate again the m_StepOneTotalApplicationCompletionTime, m_StepOneTotalApplicationEnergy
                    EnergyConsumptionAndApplicationCompletionTime(m_StepOneScheduledTasksCopy, m_StepOneTotalApplicationCompletionTime, m_StepOneTotalApplicationEnergy);
                    NewScheduledTasks.erase(NewScheduledTasks.begin(),NewScheduledTasks.end());

                    kernel_algorithm(m_StepOneScheduledTasksCopy,NewScheduledTasks,i,task,Sorg_1,Sorg_2,Sorg_3,Sorg_C);
                    EnergyConsumptionAndApplicationCompletionTime(NewScheduledTasks,NewScheduledTasksTime,NewScheduledTasksEnergy);

                    if(m_StepOneTotalApplicationCompletionTime >= NewScheduledTasksTime && m_StepOneTotalApplicationEnergy > NewScheduledTasksEnergy)
                    {
                        copyTaskValues(m_StepOneScheduledTasksCopy,NewScheduledTasks);

                    }
                    else if ((NewScheduledTasksTime <= T_max) && (NewScheduledTasksEnergy < m_StepOneTotalApplicationEnergy))
                    {
                        double NewRatio = (m_StepOneTotalApplicationEnergy - NewScheduledTasksEnergy) / (NewScheduledTasksTime - ActualStepOneApplicationTime);
                        if(NewRatio > BestRatio)
                        {
                            BestRatio = NewRatio;
                            copyTaskValues(m_StepOneScheduledTasksCopy,NewScheduledTasks);
                        }
                    }
                }
            }
        }
        updateTaskSequences(Sorg_1,Sorg_2,Sorg_3,Sorg_C,m_StepOneScheduledTasksCopy);
    }
}

void Mobile_Cloud_Computing::outerloop() {
    double Reference_Energy {};
    double Optimized_Energy {};
    double Reference_Time {};
    double T_max {};

    EnergyConsumptionAndApplicationCompletionTime(m_StepOneScheduledTasks,Reference_Time,Reference_Energy);

    T_max = 1.5 * Reference_Time;//Buffer Time
    m_StepTwoScheduledTasks = m_StepOneScheduledTasks;
    while(Optimized_Energy < Reference_Energy)
    {
        EnergyConsumptionAndApplicationCompletionTime(m_StepTwoScheduledTasks,Reference_Time,Reference_Energy);
        TaskMigration(m_StepTwoScheduledTasks,T_max);
        EnergyConsumptionAndApplicationCompletionTime(m_StepTwoScheduledTasks,Reference_Time,Optimized_Energy);
    }
}

void Mobile_Cloud_Computing::EnergyConsumptionAndApplicationCompletionTime(std::vector<Task>& TotalTaskValue, double& TotalApplicationCompletionTime, double& TotalApplicationEnergy)
{
    TotalApplicationCompletionTime = {};
    TotalApplicationEnergy = {};
    for (auto& task : TotalTaskValue)
    {
        // Calculate energy and time consumption based on channel
        switch (task.getChannelNumber())
        {

        case 0: // Core 1
            task.setTaskTimeConsumption(task.getTaskFinishTime() - task.getTaskStartTime());
            task.setTaskEnergyConsumption(m_powerCoreOneComputataion * task.getTaskTimeConsumption());
            break;

        case 1: // Core 2
            task.setTaskTimeConsumption(task.getTaskFinishTime() - task.getTaskStartTime());
            task.setTaskEnergyConsumption(m_powerCoreTwoComputataion * task.getTaskTimeConsumption());
            break;

        case 2: // Core 3
            task.setTaskTimeConsumption(task.getTaskFinishTime() - task.getTaskStartTime());
            task.setTaskEnergyConsumption(m_powerCoreThreeComputataion * task.getTaskTimeConsumption());
            break;

        case CLOUD_TASK: // Cloud Task
            // Include all cloud phases (timeSending, timeComputation, timeReceiving)
            task.setTaskTimeConsumption(task.getTaskFinishTime() - task.getTaskStartTime() - m_timeComputation - m_timeReceiving);
            task.setTaskEnergyConsumption(m_powerSending * task.getTaskTimeConsumption());
            break;

        default:
            throw std::runtime_error("Invalid channel number for energy calculation");
        }

        // Check if the task is an exit task and update TotalApplicationCompletionTime
        if (task.getisExit() && TotalApplicationCompletionTime < task.getTaskFinishTime())
        {
            TotalApplicationCompletionTime = task.getTaskFinishTime();
        }

        // Add to total energy and time for all tasks
        TotalApplicationEnergy += task.getTaskEnergyConsumption();
    }
}

void Mobile_Cloud_Computing::displayResults()
{
    std::cout << "Mobile Cloud Computing" << std::endl;
    std::cout << "Project: " << project_name << std::endl;
    std::cout << "Version: " << project_version << std::endl;
    std::cout << "-------------------------" << std::endl;

    // Validate inputs
    if (m_StepOneScheduledTasks.empty()) {
        std::cerr << "Error: Step One Scheduled Tasks are empty." << std::endl;
        return;
    }
    if (m_StepTwoScheduledTasks.empty()) {
        std::cerr << "Warning: Step Two Scheduled Tasks are empty. Displaying Step One results only." << std::endl;
    }

    double Time{};
    double Energy{};
    EnergyConsumptionAndApplicationCompletionTime(m_StepOneScheduledTasks,Time,Energy);
    std::cout<<"Step One "<<std::endl;
    std::cout<<"Energy : "<<Energy<<" Time : "<<Time<<std::endl;
    for(auto task : m_StepOneScheduledTasks)
    {std::cout << "Task " << task.getTaskNumber() << " Core: " << task.getChannelNumber()
                << " Scheduled Cloud: " << task.getisCloudTask()
                << " Start Time: " << task.getTaskStartTime()
                << " Finish Local Time: " << task.getfinishTimeLocal() <<" Finish Cloud Time "<< task.getfinishTimeWirelessReceiving() << std::endl;}

    EnergyConsumptionAndApplicationCompletionTime(m_StepTwoScheduledTasks,Time,Energy);
    std::cout<<"Step Two "<<std::endl;
    std::cout<<"Energy : "<<Energy<<" Time : "<<Time<<std::endl;
    for(auto task : m_StepTwoScheduledTasks)
    {std::cout << "Task " << task.getTaskNumber() << " Core: " << task.getChannelNumber()
                << " Scheduled Cloud: " << task.getisCloudTask()
                << " Start Time: " << task.getTaskStartTime()
                << " Finish Local Time: " << task.getfinishTimeLocal() <<" Finish Cloud Time "<< task.getfinishTimeWirelessReceiving() << std::endl;}
}
