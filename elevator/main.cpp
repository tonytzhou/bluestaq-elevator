#include <iostream>
#include <cctype>
#include "headers/Interactive.h"
#include "headers/Engineer.h"

using namespace std;

int main() {
    char modeSelection;
    cout << "Hello, welcome to my Elevator Simulation! - Tony Zhou\n"
         << "There are two modes to this, Interactive and Engineer.\n"
         << "Interactive mode - allows you to simulate elevator requests manually, essentially simulating if you were riding it yourself.\n"
         << "Engineer mode - allows you to test the elevator with a set of predefined requests to see how it handles them.\n"
         << "Please select a mode to begin: (I/E)\n";

    if (!(cin >> modeSelection)) return 0;
    modeSelection = static_cast<char>(toupper(static_cast<unsigned char>(modeSelection)));
    while (modeSelection != 'I' && modeSelection != 'E' && modeSelection != 'Q') {
        cout << "Invalid input. Please enter 'I' for Interactive mode or 'E' for Engineer mode.\n";
        if (!(cin >> modeSelection)) return 0;
        modeSelection = static_cast<char>(toupper(static_cast<unsigned char>(modeSelection)));
    }

    if (modeSelection == 'Q') {
        cout << "Exiting simulation. Goodbye!\n";
        return 0;
    } else if (modeSelection == 'I') {
        InteractiveMode();
    } else {
        EngineerMode();
    }
    return 0;
}
