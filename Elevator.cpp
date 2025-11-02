#include <queue>
#include <thread>
#include <chrono>


using namespace std;

enum Direction
{
    UP,
    DOWN,
    IDLE
};

class Elevator
{
private:
    int currentFloor;
    Direction direction;
    priority_queue<int, vector<int>, greater<int>> upQueue;
    priority_queue<int> downQueue;
    bool doorState; // t - open, f - closed
    int totalFloors;
public:
    Elevator() : currentFloor(1), direction(IDLE), doorState(false), totalFloors(20) {}
    Elevator(int totalFloors) : currentFloor(1), direction(IDLE), doorState(false), totalFloors(totalFloors) {}

    void setCurrentFloor(int floor)
    {
        currentFloor = floor;
    }
    
    int getCurrentFloor() const
    {
        return currentFloor;
    }

    int getTotalFloors() const
    {
        return totalFloors;
    }

    void step(Direction dir)
    {
        direction = dir;
        setCurrentFloor(currentFloor + (direction == UP ? 1 : -1));
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

    void outsideRequest(int floor, Direction dir)
    {
        if (direction == UP)
        {
            upQueue.push(floor);
        }
        else if (direction == DOWN)
        {
            downQueue.push(floor);
        }
    }

    void requestFloor(int floor)
    {
        if (floor > currentFloor)
        {
            upQueue.push(floor);
        }
        else if (floor < currentFloor)
        {
            downQueue.push(floor);
        }
    }

    void openDoors()
    {
        doorState = true;
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "Doors are opening for floor " << currentFloor << "." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "Doors are open." << endl;
        return;
    }

    void closeDoors()
    {
        doorState = false;
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "Doors are closing." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "Doors are closed." << endl;
        return;
    }
};