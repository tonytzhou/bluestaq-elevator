#include <iostream>
#include "headers/Interactive.h"
#include "headers/Engineer.h"
using namespace std;

int main()
{
    char modeSelection;
    cout << "Hello, welcome to my Elevator Simulation! - Tony Zhou" << endl
         << "There are two modes to this, Interactive and Engineer. " << endl
         << "Interactive mode - allows you to simulate elevator requests manually, essentially simulating if you were riding it yourself. " << endl
         << "Engineer mode - allows you to test the elevator with a set of predefined requests to see how it handles them." << endl
         << "Please select a mode to begin: (I/E) " << endl;

    cin >> modeSelection;
    modeSelection = toupper(modeSelection);
    while (modeSelection != 'I' && modeSelection != 'E' && modeSelection != 'Q')
    {
        cout << "Invalid input. Please enter 'I' for Interactive mode or 'E' for Engineer mode." << endl;
        cin >> modeSelection;
        modeSelection = toupper(modeSelection);
    }

    if (modeSelection == 'Q')
    {
        cout << "Exiting simulation. Goodbye!" << endl;
        return 0;
    }
    else if (modeSelection == 'I')
    {
        InteractiveMode();
    }
    else if (modeSelection == 'E')
    {
        EngineerMode();
    }
    return 0;
}
