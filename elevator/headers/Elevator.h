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
    // Min-heap for upward stops; max-heap (default) for downward stops
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

    // Non-blocking operations used by the engineer worker thread
    void step(Direction dir);
    void outsideRequest(int floor, Direction dir);
    void requestFloor(int floor);

    // May block briefly (prints/timing handled in .cpp)
    void openDoors();
    void closeDoors();
};

#endif // ELEVATOR_H
