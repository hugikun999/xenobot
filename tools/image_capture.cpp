#include "ros/ros.h"

#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

#include <string.h>

int num= 0;;
int count = 0;
int rate = 0;

std::string topic;

void img_cb(const sensor_msgs::Image::ConstPtr& msg)
{
	//ROS_INFO("get message");

	if(count >= num*rate) {
		ROS_INFO("Get %d images",count/rate);
		ros::shutdown();
		return;
	}

	cv_bridge::CvImagePtr cv_ptr;
	try {
		cv_ptr = cv_bridge::toCvCopy(msg,msg->encoding);
	}
	catch (cv_bridge::Exception&e) {
		ROS_ERROR("%s",e.what());
		return;
	}
	
	try {
		if(count%rate==0) {
			ROS_INFO("Saved image!");
			imwrite( "/home/ponsheng/image/"+ topic +"_" + std::to_string(count/rate) + ".jpg",cv_ptr->image );
		}
		count ++;
	}
	catch (...) {
		ROS_ERROR("image write fail");
	}
}


int main(int argc,char** argv) {
	ros::init(argc, argv, "image_capture",ros::init_options::AnonymousName);
	ros::NodeHandle pnh("~");
	std::string veh;
	//std::string topic;
	if(!pnh.getParam("veh",veh))
		exit(1);
	ROS_INFO("veh: %s",veh.c_str());

	if(!pnh.getParam("num",num))
		exit(1);
	ROS_INFO("num: %d",num);

	if(!pnh.getParam("topic",topic))
        exit(1);
	ROS_INFO("topic: %s",topic.c_str());

	if(!pnh.getParam("rate",rate))
		exit(1);
	ROS_INFO("rate: %d",rate);

	//ROS_INFO("%d\n",num);
	//ROS_INFO("%s",veh.c_str());
	ros::NodeHandle nh(veh.c_str());
	ros::Subscriber sub = nh.subscribe(topic, 5, img_cb);
	ros::spin();
	return 0;
}
