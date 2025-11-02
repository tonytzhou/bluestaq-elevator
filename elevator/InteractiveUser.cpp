#include "headers/InteractiveUser.h"

InteractiveUser::InteractiveUser()
    : directionInput(' '), userFloor(1), userState(false)
{
}

int InteractiveUser::getUserFloor() const
{
    return userFloor;
}

bool InteractiveUser::getState() const
{
    return userState;
}

void InteractiveUser::setUserFloor(int floor)
{
    userFloor = floor;
}

void InteractiveUser::setUserState(bool state)
{
    userState = state;
}
