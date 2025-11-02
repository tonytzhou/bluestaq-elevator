class InteractiveUser{
private:
        char directionInput;
        int userFloor = 1;
        bool userState = false; // false - outside, true - inside
public:
        InteractiveUser() = default;
        int getUserFloor() const {
            return userFloor;
        }
        bool getState() const {
            return userState;
        }
        void setUserFloor(int floor) {
            userFloor = floor;
        }
        void setUserState(bool state) {
            userState = state;
        }
};