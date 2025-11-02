#ifndef INTERACTIVEUSER_H
#define INTERACTIVEUSER_H

class InteractiveUser
{
private:
    char directionInput;
    int userFloor;    
    bool userState;    

public:
    InteractiveUser();

    int getUserFloor() const;
    bool getState() const;
    void setUserFloor(int floor);
    void setUserState(bool state);
};

#endif