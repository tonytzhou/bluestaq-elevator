#ifndef INTERACTIVEUSER_H
#define INTERACTIVEUSER_H

class InteractiveUser
{
private:
    char directionInput;
    int userFloor;     // current floor of user
    bool userState;    // false = outside, true = inside

public:
    InteractiveUser(); // constructor

    int getUserFloor() const;
    bool getState() const;
    void setUserFloor(int floor);
    void setUserState(bool state);
};

#endif