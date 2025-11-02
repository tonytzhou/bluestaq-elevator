#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <string>
#include <cctype>
#include <algorithm>

#include "headers/Interactive.h"

using namespace std;

static void travelDots(int steps = 3, int ms = 600)
{
    for (int i = 0; i < steps; ++i)
    {
        this_thread::sleep_for(chrono::milliseconds(ms));
        cout << ".";
        cout.flush();
    }
    cout << "\n";
}

static void printHelp(int totalFloors)
{
    cout << "\nInteractive input commands (case-insensitive):\n"
         << "  <floor> U      : Outside request at <floor> wanting to go Up\n"
         << "  Example: 7U (7th floor, UP)\n"
         << "  <floor> D      : Outside request at <floor> wanting to go Down\n"
         << "  Example: 3D (3rd floor, DOWN)\n"
         << "  <floor> N      : Inside request going to <floor>\n"
         << "  H              : Help (this menu)\n"
         << "  Q              : Quit\n"
         << "  Floors are 1.." << totalFloors << ".\n\n";
}

static int parseFloor(const string &tok)
{
    if (tok.empty())
        return -1;
    for (unsigned char c : tok)
        if (!isdigit(c))
            return -1;
    try
    {
        return stoi(tok);
    }
    catch (...)
    {
        return -1;
    }
}

static bool normalizeTokens(string &a, string &b)
{
    if (!b.empty())
        return true;
    if (a.empty())
        return false;
    unsigned char last = static_cast<unsigned char>(a.back());
    if (isalpha(last))
    {
        string digits = a.substr(0, a.size() - 1);
        if (!digits.empty() && all_of(digits.begin(), digits.end(),
                                      [](unsigned char c)
                                      { return isdigit(c); }))
        {
            b = string(1, static_cast<char>(toupper(last)));
            a = digits;
            return true;
        }
    }
    return false;
}

void stayOrExit(int floorRequest, Elevator &elevator, InteractiveUser &user)
{
    if (floorRequest == elevator.getCurrentFloor())
    {
        elevator.openDoors();
        cout << "Exit or Stay? (E/S, Q to quit)\n";
        string s;
        while (true)
        {
            if (!(cin >> s))
                return;
            if (s.empty())
                continue;
            char c = static_cast<char>(toupper(static_cast<unsigned char>(s[0])));
            if (c == 'E')
            {
                cout << "You have exited the elevator.\n";
                elevator.closeDoors();
                user.setUserState(false);
                user.setUserFloor(elevator.getCurrentFloor());
                break;
            }
            if (c == 'S')
            {
                cout << "You have chosen to stay in the elevator.\n";
                elevator.closeDoors();
                user.setUserState(true);
                user.setUserFloor(elevator.getCurrentFloor());
                break;
            }
            if (c == 'Q')
            {
                cout << "Exiting simulation. Goodbye!\n";
                exit(0);
            }
            cout << "Enter E (exit), S (stay), or Q.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void InteractiveMode()
{
    Elevator elevator;
    InteractiveUser user;
    printHelp(elevator.getTotalFloors());

    while (true)
    {
        if (!user.getState())
        {
            cout << "[Outside] You are at floor " << elevator.getCurrentFloor() << ". Enter: <floor> U/D, or H/Q\n> ";
            string a, b;
            if (!(cin >> a))
            {
                cout << "Exiting.\n";
                return;
            }

            if (a.size() == 1)
            {
                char c = static_cast<char>(toupper(static_cast<unsigned char>(a[0])));
                if (c == 'Q')
                {
                    cout << "Exiting simulation. Goodbye!\n";
                    return;
                }
                if (c == 'H')
                {
                    printHelp(elevator.getTotalFloors());
                    continue;
                }
                if (c == 'P')
                {
                    cout << "[Status] Current floor: " << elevator.getCurrentFloor()
                         << " / Total floors: " << elevator.getTotalFloors() << "\n";
                    continue;
                }
            }

            b.clear();
            if (!normalizeTokens(a, b))
            {
                if (!(cin >> b))
                {
                    cout << "Expected direction (U/D).\n";
                    cin.clear();
                    continue;
                }
            }

            int hallFloor = parseFloor(a);
            if (hallFloor < 1 || hallFloor > elevator.getTotalFloors())
            {
                cout << "Invalid floor.\n";
                continue;
            }
            char dirChar = static_cast<char>(toupper(static_cast<unsigned char>(b[0])));
            if (dirChar != 'U' && dirChar != 'D')
            {
                cout << "Use U or D.\n";
                continue;
            }
            Direction hallDir = (dirChar == 'U') ? UP : DOWN;

            int car = elevator.getCurrentFloor();
            if (hallDir == DOWN && hallFloor >= car)
            {
                cout << "Currently moving DOWN. That DOWN call at floor " << hallFloor
                     << " will be served on the next DOWN pass.\n";
            }
            if (hallDir == UP && hallFloor <= car)
            {
                cout << "Currently moving UP. That UP call at floor " << hallFloor
                     << " will be served on the next UP pass.\n";
            }

            if (hallFloor == car)
            {
                elevator.openDoors();
                cout << "You have entered the elevator.\n";
                user.setUserState(true);
                user.setUserFloor(hallFloor);
                elevator.closeDoors();
            }
            else
            {
                elevator.outsideRequest(hallFloor, hallDir);
                while (elevator.getCurrentFloor() != hallFloor)
                {
                    int before = elevator.getCurrentFloor();
                    elevator.step(hallDir);
                    int dest = elevator.getCurrentFloor();
                    if (dest != before)
                    {
                        if (dest > before)
                            cout << "Elevator moving up from " << before << " to " << dest << " ";
                        else
                            cout << "Elevator moving down from " << before << " to " << dest << " ";
                        travelDots();
                        if (dest == hallFloor)
                        {
                            elevator.openDoors();
                            cout << "You have entered the elevator.\n";
                            user.setUserState(true);
                            user.setUserFloor(dest);
                            elevator.closeDoors();
                            break;
                        }
                        else
                        {
                            elevator.openDoors();
                            elevator.closeDoors();
                        }
                    }
                    else
                    {
                        this_thread::sleep_for(chrono::milliseconds(120));
                    }
                }
            }
        }
        else
        {
            cout << "[Inside] Current floor " << elevator.getCurrentFloor()
                 << ". Enter destination (<floor> or <floor>N), or P/H/Q\n> ";
            string a, b;
            if (!(cin >> a))
            {
                cout << "Exiting.\n";
                return;
            }

            if (a.size() == 1)
            {
                char c = static_cast<char>(toupper(static_cast<unsigned char>(a[0])));
                if (c == 'Q')
                {
                    cout << "Exiting simulation. Goodbye!\n";
                    return;
                }
                if (c == 'H')
                {
                    printHelp(elevator.getTotalFloors());
                    continue;
                }
                if (c == 'P')
                {
                    cout << "[Status] Current floor: " << elevator.getCurrentFloor()
                         << " / Total floors: " << elevator.getTotalFloors() << "\n";
                    continue;
                }
            }

            b.clear();
            normalizeTokens(a, b);
            int destFloor = parseFloor(a);
            if (destFloor < 1 || destFloor > elevator.getTotalFloors())
            {
                cout << "Invalid floor.\n";
                continue;
            }

            if (destFloor == elevator.getCurrentFloor())
            {
                stayOrExit(destFloor, elevator, user);
                continue;
            }

            Direction pref = (destFloor > elevator.getCurrentFloor()) ? UP : DOWN;
            elevator.requestFloor(destFloor);

            while (elevator.getCurrentFloor() != destFloor)
            {
                int before = elevator.getCurrentFloor();
                elevator.step(pref);
                int after = elevator.getCurrentFloor();
                if (after != before)
                {
                    if (after > before)
                        cout << "Elevator moving up from " << before << " to " << after << " ";
                    else
                        cout << "Elevator moving down from " << before << " to " << after << " ";
                    travelDots();
                    elevator.openDoors();
                    elevator.closeDoors();
                }
                else
                {
                    this_thread::sleep_for(chrono::milliseconds(120));
                }
            }

            cout << "Elevator has arrived at floor " << elevator.getCurrentFloor() << ".\n";
            stayOrExit(destFloor, elevator, user);
        }
    }
}
