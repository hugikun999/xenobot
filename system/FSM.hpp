#include "xenobot/ModeSwitch.h"

enum ControllerMode {
        JOYSTICK_MODE,
        SELF_DRIVING_MODE,
        STOP_MODE,

	NUM_OF_MODE
};

class FSM {
    private:
	int available[NUM_OF_MODE][NUM_OF_MODE] = {{0}};
	ros::ServiceServer serv_mode_switch;

    public:
    int mode;
    FSM();
    int next(int next_mode);
};
