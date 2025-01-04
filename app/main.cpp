#include <iostream>
#include "Mobile_Cloud_Computing.hpp"
#include <vector>

int main() {

    // Define task core values
    std::vector<std::vector<double>> coreValues = {
        {9, 7, 5}, {8, 6, 5}, {6, 5, 4}, {7, 5, 3},
        {5, 4, 2}, {7, 6, 4}, {8, 5, 3}, {6, 4, 2},
        {5, 3, 2}, {7, 4, 2}};

    // Define task graph
    std::vector<std::pair<int, int>> taskGraph = {
        {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
        {2, 8}, {2, 9}, {3, 7}, {4, 8}, {4, 9},
        {5, 9}, {6, 8}, {7, 10}, {8, 10}, {9, 10}};

    // Cloud phase parameters
    constexpr double TIME_SENDING = 3.0;
    constexpr double TIME_COMPUTATION = 1.0;
    constexpr double TIME_RECEIVING = 1.0;

    // Power parameters
    constexpr double POWER_SENDING = 0.5;
    constexpr double POWER_CORE_ONE = 1.0;
    constexpr double POWER_CORE_TWO = 2.0;
    constexpr double POWER_CORE_THREE = 4.0;

    Mobile_Cloud_Computing mcc(coreValues, taskGraph, TIME_SENDING, TIME_COMPUTATION, TIME_RECEIVING,
    POWER_SENDING, POWER_CORE_ONE, POWER_CORE_TWO, POWER_CORE_THREE);

    mcc.runStepOneScheduling();
    mcc.runStepTwoScheduling();
    mcc.displayResults();

    return 0;
}
