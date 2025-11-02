#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <queue>
#include <vector>
#include <functional>

enum Direction { UP, DOWN, IDLE };

class Elevator {
private:
    int currentFloor{1};
    Direction direction{IDLE};
    std::priority_queue<int, std::vector<int>, std::greater<int>> upQueue;
    std::priority_queue<int> downQueue;
    bool doorState{false};
    int totalFloors{20};

public:
    Elevator();
    explicit Elevator(int totalFloors);

    void setCurrentFloor(int floor);
    int  getCurrentFloor() const;
    int  getTotalFloors() const;

    void step(Direction dir);
    void outsideRequest(int floor, Direction dir);
    void requestFloor(int floor);

    void openDoors();
    void closeDoors();
};

#endif 
