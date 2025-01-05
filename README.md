# Mobile Cloud Computing Task Scheduler
An efficient task scheduling framework for mobile cloud computing environments, focusing on energy efficiency and performance under hard deadlines.

## Disclaimer
- This implementation is inspired by the kernel algorithm described in the research paper [Energy-Efficient Scheduling in MCC](https://ieeexplore.ieee.org/document/6973741). However, please note that due to rounding-off mechanisms in certain start-time and finish-time computations, the outputs of this implementation might differ slightly from those presented in the paper.

## Boundary Conditions Validation
- The implementation has been rigorously tested against the boundary conditions specified in the algorithm. These conditions ensure that all tasks are scheduled within the constraints of energy and time while meeting the required deadlines.

### Example: 1st Stage Scheduling of the Kernel Algorithm
![image](https://github.com/user-attachments/assets/932da080-64b5-40b1-b435-aeba5bcae606)

### Behavior without `T_max` (Hard Deadline)
- After removing the `T_max` value, the system demonstrates flexibility by scheduling all tasks to the cloud, optimizing energy savings. This aligns with the expected behavior of the algorithm:
  - **Energy**: Energy savings are maximized by shifting computation to the cloud.
![image](https://github.com/user-attachments/assets/b3eae672-bb6b-4afa-b692-99529716fe90)

This demonstrates that the implementation satisfies the boundary conditions and behaves as expected under varying deadlines.

## Features
- Energy-efficient task scheduling
- Supports heterogeneous cores and cloud offloading
- Simulation results demonstrate significant energy savings

## Background
Mobile devices often face challenges like limited computing resources and short battery life. This project leverages Mobile Cloud Computing (MCC) to offload tasks to the cloud, ensuring performance improvement and energy efficiency.

## Getting Started
To get you a copy of the project up and running on your local machine for development and testing purposes, please follow the steps provided.

### Software Requirements
- CMake 3.21+
- MSVC 2017 (or higher), G++9 (or higher), Clang++9 (or higher)
- Optional: Makefile

### Building
- First, clone this repo and do the preliminary work:
- git clone https://github.com/Kabhilesh-Giri/MobileCloudComputing-Task-Scheduler.git

[Build the Project]
- make prepare
- cd build/app
- ./Executable

## Actual Output
![image](https://github.com/user-attachments/assets/7bd7a904-9bd3-4c33-adc6-66fc08e860b8)

## Contributing
Contributions are welcome!
Please fork the repository, make your changes, and create a pull request.

## Contact
For any inquiries, please email [kabhilesh20@gmail.com].

## Reference
1. [Energy-Efficient Scheduling in MCC](https://ieeexplore.ieee.org/document/6973741)




