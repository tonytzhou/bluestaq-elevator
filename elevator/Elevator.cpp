// Elevator.cpp
#include "headers/Elevator.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <vector>
#include <functional>
#include <climits>

using namespace std;

Elevator::Elevator()
    : currentFloor(1),
      direction(IDLE),
      doorState(false),
      totalFloors(20) {}

Elevator::Elevator(int totalFloors)
    : currentFloor(1),
      direction(IDLE),
      doorState(false),
      totalFloors(totalFloors) {}

void Elevator::setCurrentFloor(int floor)
{
    if (floor < 1)
        floor = 1;
    if (floor > totalFloors)
        floor = totalFloors;
    currentFloor = floor;
}

int Elevator::getCurrentFloor() const { return currentFloor; }
int Elevator::getTotalFloors() const { return totalFloors; }

void Elevator::outsideRequest(int floor, Direction dir)
{
    if (floor < 1 || floor > totalFloors)
        return;
    if (floor == currentFloor)
    {
        openDoors();
        closeDoors();
        return;
    }
    if (dir == UP)
    {
        upQueue.push(floor);
        if (!(floor > currentFloor))
        {
            cout << "UP call at floor " << floor << " will be served on the next UP pass.\n";
        }
        else if (direction != UP)
        {
            cout << "UP call queued. Will be served when the car goes UP.\n";
        }
    }
    else if (dir == DOWN)
    {
        downQueue.push(floor);
        if (!(floor < currentFloor))
        {
            cout << "[Hall] DOWN call at floor " << floor << " will be served on the next DOWN pass.\n";
        }
        else if (direction != DOWN)
        {
            cout << "[Hall] DOWN call queued. Will be served when the car goes DOWN.\n";
        }
    }
    else
    {
        if (floor > currentFloor)
        {
            upQueue.push(floor);
            cout << "[Hall] Inferred UP call at floor " << floor << " (served when car goes UP).\n";
        }
        else
        {
            downQueue.push(floor);
            cout << "[Hall] Inferred DOWN call at floor " << floor << " (served when car goes DOWN).\n";
        }
    }
}

void Elevator::requestFloor(int floor)
{
    if (floor < 1 || floor > totalFloors)
        return;
    if (floor == currentFloor)
    {
        openDoors();
        closeDoors();
        return;
    }
    if (direction == DOWN)
    {
        if (floor < currentFloor)
        {
            downQueue.push(floor);
        }
        else
        {
            cout << "[Inside] Currently moving DOWN. Destination " << floor
                 << " is at/above current floor " << currentFloor
                 << " and will be served on the next UP pass.\n";
            upQueue.push(floor);
        }
    }
    else if (direction == UP)
    {
        if (floor > currentFloor)
        {
            upQueue.push(floor);
        }
        else
        {
            cout << "[Inside] Currently moving UP. Destination " << floor
                 << " is at/below current floor " << currentFloor
                 << " and will be served on the next DOWN pass.\n";
            downQueue.push(floor);
        }
    }
    else
    {
        if (floor > currentFloor)
            upQueue.push(floor);
        else
            downQueue.push(floor);
    }
}

void Elevator::step(Direction preferred)
{
    auto takeUp = [&]() -> bool
    {
        if (upQueue.empty())
            return false;
        vector<int> items;
        while (!upQueue.empty())
        {
            items.push_back(upQueue.top());
            upQueue.pop();
        }
        int bestAbove = INT_MAX;
        int bestBelowEq = INT_MIN;
        for (int f : items)
        {
            if (f > currentFloor)
                bestAbove = min(bestAbove, f);
            else
                bestBelowEq = max(bestBelowEq, f);
        }
        int target = (bestAbove != INT_MAX) ? bestAbove
                                            : (bestBelowEq != INT_MIN ? bestBelowEq : -1);
        if (target == -1)
        {
            for (int f : items)
                upQueue.push(f);
            return false;
        }
        for (int f : items)
            if (f != target)
                upQueue.push(f);
        currentFloor = target;
        direction = UP;
        return true;
    };

    auto takeDown = [&]() -> bool
    {
        if (downQueue.empty())
            return false;
        vector<int> items;
        while (!downQueue.empty())
        {
            items.push_back(downQueue.top());
            downQueue.pop();
        }
        int bestBelow = INT_MIN;
        int bestAboveEq = INT_MAX;
        for (int f : items)
        {
            if (f < currentFloor)
                bestBelow = max(bestBelow, f);
            else
                bestAboveEq = min(bestAboveEq, f);
        }
        int target = (bestBelow != INT_MIN) ? bestBelow
                                            : (bestAboveEq != INT_MAX ? bestAboveEq : -1);
        if (target == -1)
        {
            for (int f : items)
                downQueue.push(f);
            return false;
        }
        for (int f : items)
            if (f != target)
                downQueue.push(f);
        currentFloor = target;
        direction = DOWN;
        return true;
    };

    bool moved = false;
    if (preferred == UP)
    {
        moved = takeUp();
    }
    else if (preferred == DOWN)
    {
        moved = takeDown();
    }
    else
    {
        moved = takeUp() || takeDown();
    }
    if (!moved)
        direction = IDLE;
}

void Elevator::openDoors()
{
    doorState = true;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Doors are opening for floor " << currentFloor << ".\n";
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Doors are open.\n";
}

void Elevator::closeDoors()
{
    doorState = false;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Doors are closing.\n";
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Doors are closed.\n";
}
