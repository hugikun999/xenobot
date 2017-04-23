#include "ros/ros.h"
#include "xenobot/RegNode.h"

bool register_node(std::string machine_name) {
	ros::NodeHandle n;
	std::string srv_name = "/server/register_node";
	ros::ServiceClient client = n.serviceClient<xenobot::RegNode>(srv_name);

	xenobot::RegNode srv;
	srv.request.name = machine_name;
	if(client.call(srv)) {
		if(srv.response.success)	{
			ROS_INFO("p1");
			return true;
		}
	} else {
		ROS_ERROR("Fail to call service %s",srv_name.c_str());
	}
	ROS_INFO("p2");
	return false;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "client");
	ROS_INFO("Initializing");

	if(register_node("hello")) {
		ROS_INFO("Register to server!");
	} else {
		ROS_ERROR("Fail to register");
	}
	ROS_INFO("Registerededededededededed");
	ros::spin();
  return 0;
}
