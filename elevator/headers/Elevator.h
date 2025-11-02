#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <queue>
#include <thread>
#include <chrono>
using namespace std;

enum Direction { UP, DOWN, IDLE };

class Elevator {
private:
    int currentFloor;
    Direction direction;
    priority_queue<int, vector<int>, greater<int>> upQueue;
    priority_queue<int> downQueue;
    bool doorState;
    int totalFloors;

public:
    Elevator();
    Elevator(int totalFloors);

    void setCurrentFloor(int floor);
    int getCurrentFloor() const;
    int getTotalFloors() const;
    void step(Direction dir);
    void outsideRequest(int floor, Direction dir);
    void requestFloor(int floor);
    void openDoors();
    void closeDoors();
};

#endif