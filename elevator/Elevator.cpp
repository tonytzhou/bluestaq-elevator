#include "headers/Elevator.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
using namespace std;

Elevator::Elevator() : currentFloor(1), direction(IDLE), doorState(false), totalFloors(20) {}

Elevator::Elevator(int totalFloors)
    : currentFloor(1), direction(IDLE), doorState(false), totalFloors(totalFloors) {}

void Elevator::setCurrentFloor(int floor)
{
    currentFloor = floor;
}

int Elevator::getCurrentFloor() const
{
    return currentFloor;
}

int Elevator::getTotalFloors() const
{
    return totalFloors;
}

void Elevator::step(Direction dir)
{
    direction = dir;
    if (direction == UP && !upQueue.empty())
    {
        currentFloor = upQueue.top();
        upQueue.pop();
    }
    else if (direction == DOWN && !downQueue.empty())
    {
        currentFloor = downQueue.top();
        downQueue.pop();
    }
}

void Elevator::outsideRequest(int floor, Direction dir)
{
    if (dir == UP)
    {
        upQueue.push(floor);
    }
    else if (dir == DOWN)
    {
        downQueue.push(floor);
    }
}

void Elevator::requestFloor(int floor)
{
    if (floor == currentFloor)
    {
        openDoors();
        closeDoors();
    }
    else if (floor > currentFloor)
    {
        upQueue.push(floor);
    }
    else if (floor < currentFloor)
    {
        downQueue.push(floor);
    }
}

void Elevator::openDoors()
{
    doorState = true;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Doors are opening for floor " << currentFloor << "." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Doors are open." << endl;
}

void Elevator::closeDoors()
{
    doorState = false;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Doors are closing." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Doors are closed." << endl;
}
