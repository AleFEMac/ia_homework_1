#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include <string>
#include <sstream>
#include <iostream>

void distanceCallback(const sensor_msgs::LaserScan::ConstPtr& msg, float* min) {
	*min = msg->ranges[0];
	int len = msg->ranges.size();
	for(int i=0; i<len; ++i)
		if(msg->ranges[i] < *min)
			*min = msg->ranges[i];
}

int main(int argc, char **argv) {

	ros::init(argc, argv, "minnode");

	ros::NodeHandle n;  
	float min;
	
	ros::Publisher pub = n.advertise<std_msgs::String>("mintopic", 1000);
	ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("base_scan", 1000, boost::bind(distanceCallback, _1, &min));
	ros::Rate loop_rate(10);

	while (ros::ok()) {
		std_msgs::String msg;
		std::stringstream ss;
		ss << "closest obstacle at distance " << min;
		msg.data = ss.str();
		pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
