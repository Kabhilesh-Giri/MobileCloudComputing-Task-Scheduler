# Mobile Cloud Computing Task Scheduler
An efficient task scheduling framework for mobile cloud computing environments, focusing on energy efficiency and performance under hard deadlines.

## Disclaimer
- This implementation is inspired by the kernel algorithm described in the research paper [Energy-Efficient Scheduling in MCC](https://ieeexplore.ieee.org/document/6973741). However, please note that due to rounding-off mechanisms in certain start-time and finish-time computations, the outputs of this implementation might differ slightly from those presented in the paper.

## Boundary Conditions Validation
- The implementation has been rigorously tested against the boundary conditions specified in the algorithm. These conditions ensure that all tasks are scheduled within the constraints of energy and time while meeting the required deadlines.

### Example: 1st Stage Scheduling of the Kernel Algorithm
![image](https://github.com/user-attachments/assets/eab7afcd-5577-4fef-9177-4a02224d8436)

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

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/username/MobileCloudComputing-Task-Scheduler.git
   cd MobileCloudComputing-Task-Scheduler
   make prepare

   Need to update

## Usage
To run the task scheduler, execute the following command:
./scheduler input_task_graph.txt
Need to update

## Contributing
Contributions are welcome! Please fork the repository and create a pull request.

## Contact
For any inquiries, please email [kabhilesh20@gmail.com].

## Reference
1. [Energy-Efficient Scheduling in MCC](https://ieeexplore.ieee.org/document/6973741)




