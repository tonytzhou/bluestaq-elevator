# Bluestaq Elevator Simulation C++
Integrated terminal GUI interface into Elevator Backend system that supports two modes, Interactive and Engineer.

# Prereqs
C++ 17 compiler (I used GCC)

# Usage
*Build*
```bash
git clone https://github.com/tonytzhou/bluestaq-elevator
cd elevator
```

*Run*
```bash
g++ main.cpp Elevator.cpp Interactive.cpp Engineer.cpp InteractiveUser.cpp -o elevator
./elevator.exe
```


# Overview
Interactive Mode - Simulates actually running through the elevator as an actual person, can make calls to certain floors and go in/out of the Elevator if wanted.

Engineer Mode - Simulates going to an engineer and can assign the elevator where to go/ how it would interact given different inputs. Done in parallel to simulate multiple inputs at once, can also input commands when the elevator is running, simulating a real life elevator experience. 

# Implementation
Parallel program that uses 2 priority queues for main logic in order to handle calls for elevator getting up/down requests at differnet time. The 2 priority queues serve as max/min heaps that allow the elevator to know where the user wants to head to wand which floors take priority over one another. Supports error handling if user enters a wrong input as well.

# Assumptions
- Elevator can handle multiple calls at once
- Elevator always starts at floor 1
- No set floor for elevator in engineer mode, for engineers to decide how many floors an elevator should have
- When I think of "simulation", I think of actually going into and testing it, hence interactive mode
- Elevator has no weight limit
- Elevator simulation code should be scalable
- Currently only one elevator car


# Future Implementations
- Elevator Weight Limit
- Emergency button elevator getting stuck
- Open/Close Elevator buttons
- Converting C++ to Java (done in order to maximize runtime & parallel efficiency)
- Door sensors, making sure doesn't close on people 
