#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include "headers/Interactive.h"
using namespace std;

void stayOrExit(int floorRequest, Elevator &elevator, InteractiveUser &user) //prompts user to stay or exit at a requested
{
    if (floorRequest == elevator.getCurrentFloor())
    {
        elevator.openDoors();
        cout << "Do you want to exit or stay in the elevator?" << endl;
        cout << "Options: Exit/Stay (E/S)" << endl;
        string exitChoice;

        while (true)
        {
            cin >> exitChoice;
            if (exitChoice.empty())
                continue;
            exitChoice[0] = toupper(exitChoice[0]);

            if (exitChoice.size() == 1 && (exitChoice[0] == 'E' || exitChoice[0] == 'S' || exitChoice[0] == 'Q'))
            {
                if (exitChoice[0] == 'E')
                {
                    cout << "You have exited the elevator." << endl;
                    elevator.closeDoors();
                    user.setUserState(false);
                }
                else if (exitChoice[0] == 'S')
                {
                    cout << "You have chosen to stay in the elevator." << endl;
                    elevator.closeDoors();
                    user.setUserState(true);
                }
                else if (exitChoice[0] == 'Q')
                {
                    cout << "Exiting simulation. Goodbye!" << endl;
                    exit(0);
                }
                break;
            }
            else
            {
                cout << "Invalid input. Please enter 'E' to Exit or 'S' to Stay." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }
}

void InteractiveMode()
{
    Elevator elevator;
    char continueChoice = 'C';
    cout << "You have selected Interactive Mode." << endl
         << endl;
    InteractiveUser user;
    int floorRequest;
    string directionInput;

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

        if (user.getState() == false)
        {
            if (elevator.getCurrentFloor() == 1)
            {
                cout << "Options: Up (U)" << endl;
            }
            else if (elevator.getCurrentFloor() == elevator.getTotalFloors())
            {
                cout << "Options: Down (D)" << endl;
            }
            else
            {
                cout << "Options: Up/Down (U/D)" << endl;
            }

            bool validDir = false;
            while (!validDir)
            {
                cin >> directionInput;
                if (directionInput.empty())
                    continue;
                directionInput[0] = toupper(directionInput[0]);

                if (directionInput.size() != 1)
                {
                    cout << "Invalid input. Please enter 'U' for Up or 'D' for Down." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                if (directionInput[0] == 'Q')
                {
                    cout << "Exiting simulation. Goodbye!" << endl;
                    exit(0);
                }
                else if (directionInput[0] == 'U' && elevator.getCurrentFloor() == elevator.getTotalFloors())
                {
                    cout << "You are on the top floor and cannot go up. Please enter 'D' for Down." << endl;
                }
                else if (directionInput[0] == 'D' && elevator.getCurrentFloor() == 1)
                {
                    cout << "You are on the ground floor and cannot go down. Please enter 'U' for Up." << endl;
                }
                else if (directionInput[0] == 'U' || directionInput[0] == 'D')
                {
                    validDir = true;
                }
                else
                {
                    cout << "Invalid input. Please enter 'U' for Up or 'D' for Down." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
        }

        Direction dir = (toupper(directionInput[0]) == 'U') ? UP : DOWN;

        elevator.outsideRequest(floorRequest, dir);

        if (user.getUserFloor() == elevator.getCurrentFloor() && user.getState() == false)
        {
            elevator.openDoors();
            cout << "You have entered the elevator." << endl;
            user.setUserState(true);
            elevator.closeDoors();
        }

        cout << "Enter the floor you want to go to: (1-" << elevator.getTotalFloors() << "): ";
        while (!(cin >> floorRequest) || floorRequest < 1 || floorRequest > elevator.getTotalFloors())
        {
            cout << "Invalid floor request. Please enter a floor between 1 and " << elevator.getTotalFloors() << "." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (floorRequest == elevator.getCurrentFloor())
        {
            stayOrExit(floorRequest, elevator, user);
            continue;
        }

        elevator.requestFloor(floorRequest);

        while (elevator.getCurrentFloor() != floorRequest)
        {
            if (elevator.getCurrentFloor() < floorRequest)
            {
                elevator.step(UP);
                cout << "Elevator is moving up to floor " << floorRequest << "." << endl;
                for (int i = 0; i < 3; i++)
                {
                    this_thread::sleep_for(chrono::milliseconds(700));
                    cout << ".";
                }
                elevator.setCurrentFloor(floorRequest);
                user.setUserFloor(floorRequest);
                cout << endl;
                break;
            }
            else if (elevator.getCurrentFloor() > floorRequest)
            {
                elevator.step(DOWN);
                cout << "Elevator is moving down to floor " << floorRequest << "." << endl;
                for (int i = 0; i < 3; i++)
                {
                    this_thread::sleep_for(chrono::milliseconds(700));
                    cout << ".";
                }
                elevator.setCurrentFloor(floorRequest);
                user.setUserFloor(floorRequest);
                cout << endl;
                break;
            }
        }

        cout << "Elevator has arrived at floor " << elevator.getCurrentFloor() << "." << endl;
        stayOrExit(floorRequest, elevator, user);

    } while (true);
}
