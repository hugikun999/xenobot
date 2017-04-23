#include "ros/ros.h"
#include <yaml-cpp/yaml.h>

#include "xenobot/RegNode.h"
#include "xenobot/RoadReq.h"
#include "xenobot/ModeSwitch.h"

#include "server.hpp"
#include "FSM.hpp"

using namespace std;

class node {
private:
	string name;
public:
	node() {
	}
};

/*class map_pixel {
private:
	int id


}*/

int lock = 0;
std::string last_one;
ros::NodeHandle *nh;

bool register_node(xenobot::RegNode::Request  &req,
         xenobot::RegNode::Response &res)
{
	ROS_INFO("%s registered", req.name.c_str());
	res.success = true;
	return true;
}

bool road_req(xenobot::RoadReq::Request  &req,
         xenobot::RoadReq::Response &res)
{
    ROS_INFO("%s request type:%d !", req.name.c_str(),req.type);
    
    if(req.type == LOCK) {
		if(lock==0) {
			lock++;
			res.success = true;
			res.mode = SELF_DRIVING_MODE;
    	} else {
			lock++;
			last_one = req.name.c_str();
			res.success = false;
			res.mode = STOP_MODE;
    	}

	} else if(req.type == REL) {

		lock --;	
		if(lock >0) {
			//send
			std::string srv_name = "/server/register_node";
    		ros::ServiceClient client = nh->serviceClient<xenobot::RegNode>(srv_name);

    		xenobot::ModeSwitch srv;
    		srv.request.mode = SELF_DRIVING_MODE;
    		if(client.call(srv)) {
        		if(srv.response.success)    {
            		ROS_INFO("changed");
            		return true;
        		}
    		}
        	ROS_ERROR("Fail to call service %s",srv_name.c_str());
    		

		}

	}

    return true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "server");
	ros::NodeHandle nh_t("server");
	nh = &nh_t;

	ros::ServiceServer serv_register_node = nh->advertiseService("register_node", register_node);
	ROS_INFO("Ready for register!");

	string map_path = "/home/ponsheng/catkin_ws/src/xenobot/config/xenobot/map.yaml";

	try {
		YAML::Node config = YAML::LoadFile(map_path);
		YAML::Node seq2 = config["map"];
		YAML::Node seq1 = seq2[1];
		ROS_INFO("Load file successful %d",(seq1[1].as<int>()));
	}
	catch(...) {
		ROS_ERROR("Failed to load file ");
	}

    ros::ServiceServer serv_road_req = nh->advertiseService("/road_req", road_req);


	ros::spin();

	return 0;
}
