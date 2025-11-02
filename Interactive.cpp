#include <iostream>
#include "Elevator.cpp"
#include "InteractiveUser.cpp"
using namespace std;

void stayOrExit(int floorRequest, Elevator &elevator, InteractiveUser &user)
{
    if (floorRequest == elevator.getCurrentFloor())
    {
        elevator.openDoors();
        cout << "Do you want to exit or stay in the elevator?" << endl;
        cout << "Options: Exit/Stay/Quit Simulation (E/S/Q)" << endl;
        char exitChoice;
        cin >> exitChoice;
        if (toupper(exitChoice) == 'E')
        {
            cout << "You have exited the elevator." << endl;
            elevator.closeDoors();
            user.setUserState(false);
        }
        else if (toupper(exitChoice) == 'S')
        {
            cout << "You have chosen to stay in the elevator." << endl;
            elevator.closeDoors();
            user.setUserState(true);
        }
        else
        {
            cout << "Exiting simulation. Goodbye!" << endl;
            exit(0);
        }
    }
}

void InteractiveMode()
{
    Elevator elevator;
    char continueChoice;
    cout << "You have selected Interactive Mode." << endl
         << endl;
    InteractiveUser user;
    int floorRequest;
    char directionInput;
    do
    {
        if (user.getState() == true)
        {
            cout << "You are currently inside the elevator at floor " << elevator.getCurrentFloor() << "." << endl;
        }
        else
        {
            cout << "You are currently outside the elevator at floor " << elevator.getCurrentFloor() << "." << endl;
        }

        if (elevator.getCurrentFloor() == 1 && user.getState() == false)
        {
            cout << "Options: Up" << endl;
            cout << "Enter direction (U for Up): ";
        }
        else if (user.getState() == false)
        {
            cout << "Options: Up/Down" << endl;
            cout << "Enter direction (U for Up, D for Down): ";
        }

        do
        {
            cin >> directionInput;
            directionInput = toupper(directionInput);
            if (directionInput == 'Q')
            {
                cout << "Exiting simulation. Goodbye!" << endl;
                exit(0);
            }
            else if (directionInput != 'U' && directionInput != 'D')
            {
                cout << "Invalid input. Please enter 'U' for Up or 'D' for Down" << endl;
            }
        } while (directionInput != 'U' && directionInput != 'D');

        Direction dir = (directionInput == 'U') ? UP : DOWN;

        elevator.outsideRequest(floorRequest, dir);

        if (user.getUserFloor() == elevator.getCurrentFloor() && user.getState() == false)
        {
            elevator.openDoors();
            cout << "You have entered the elevator." << endl;
            user.setUserState(true);
            elevator.closeDoors();
        }

        cout << "Enter the floor you want to go to: (1-" << elevator.getTotalFloors() << "): ";
        cin >> floorRequest;

        if (floorRequest < 1 || floorRequest > elevator.getTotalFloors())
        {
            cout << "Invalid floor request. Please enter a floor between 1 and 20." << endl;
            cin >> floorRequest;
        }
        else if (floorRequest == elevator.getCurrentFloor())
        {
            stayOrExit(floorRequest, elevator, user);
        }

        elevator.requestFloor(floorRequest);

        while (elevator.getCurrentFloor() != floorRequest)
        {
            if (elevator.getCurrentFloor() < floorRequest)
            {
                elevator.step(UP);
                cout << "Elevator is moving up to floor " << elevator.getCurrentFloor() << "." << endl;
            }
            else if (elevator.getCurrentFloor() > floorRequest)
            {
                elevator.step(DOWN);
                cout << "Elevator is moving down to floor " << elevator.getCurrentFloor() << "." << endl;
            }
        }

        cout << "Elevator has arrived at floor " << elevator.getCurrentFloor() << "." << endl;
        stayOrExit(floorRequest, elevator, user);
    } while (toupper(continueChoice) != 'Q' || toupper(floorRequest) != 'Q' || toupper(directionInput) != 'Q');
}