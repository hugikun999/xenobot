#include <iostream>
#include "ros/ros.h"

#include "FSM.hpp"

/*enum ControllerMode {
        JOYSTICK_MODE,
        SELF_DRIVING_MODE,
        STOP_MODE
};

class FSM {
    private:
	cur_state;

    public:
    next(int mode);

}*/

extern ros::NodeHandle *nh;
extern FSM *fsm;

bool mode_switch(xenobot::ModeSwitch::Request  &req,
         xenobot::ModeSwitch::Response &res)
{
    std::cout << "callback!!!\n\n\n\n";
    fsm->next(req.mode);
    res.success = true;
    return true;
}

FSM::FSM() {
    mode = STOP_MODE;
    available[STOP_MODE][SELF_DRIVING_MODE] = 1;
    available[SELF_DRIVING_MODE][STOP_MODE] = 1;
    serv_mode_switch = nh->advertiseService("mode_switch", mode_switch);
}

int FSM::next(int next_mode) {

    if(mode == next_mode) {
	return 0;
    }

 //   if(available[mode][next_mode]) {
        mode = next_mode;
	std::cout << "mode change to %d success"<<mode << std::endl;

	return 0;
  //  } else {
        std::cout << "mode change failed"<< std::endl;
	return -1;
    //}
}

