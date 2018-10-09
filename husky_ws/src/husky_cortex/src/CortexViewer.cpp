/*
 * Creating a node which showes meshes in rviz
 */

// %Tag(FULLTEXT)%
// %Tag(INCLUDES)%
#include <ros/ros.h>
#include <string>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>

int main( int argc, char** argv )
{
    //Initializes ROS, and sets up a node
    ros::init(argc, argv, "cortexViewer");

    srand(time(0));
    //ros::Subscriber sub = n.subscribe("/husky_velocity_controller/odom", 1, &Listener::callback, &listener);
    ros::Rate loop_rate(10);
    while(ros::ok()){
      ros::spinOnce();
      loop_rate.sleep();
    }
}