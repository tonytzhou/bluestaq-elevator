#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include "Elevator.h"
#include "InteractiveUser.h"

void stayOrExit(int floorRequest, Elevator &elevator, InteractiveUser &user);
void InteractiveMode();

#endif