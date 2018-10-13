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

double goalx, goaly;

class TerrainNavigator;
class OdomTopicHandle;
class OdomTopicHandle{
  public:
  std::string topic_;
  ros::NodeHandle node_;
  TerrainNavigator *tnav_;
  ros::Time stamp_;
  bool is_latest_; // whether there is a new
  nav_msgs::OdometryConstPtr* lastmsg_ptr_;
  ros::Subscriber subscriber_;
  public:
  OdomTopicHandle(TerrainNavigator* tnav){ // terrain navigator pointer
    is_latest_ = false;
    node_=ros::NodeHandle();
    topic_ = "/husky_velocity_controller/odom";
    lastmsg_ptr_ = NULL;
    subscriber_ = node_.subscribe(topic_, 1, &OdomTopicHandle::odomCallback, this);
    
  }
  void odomCallback(const nav_msgs::OdometryConstPtr& msg){
    
    ROS_INFO("ODOMETRY pose: (%lf, %lf, %lf)",msg->pose.pose.position.x,msg->pose.pose.position.y,msg->pose.pose.position.z);
    if(lastmsg_ptr_==NULL){
      lastmsg_ptr_ = new nav_msgs::OdometryConstPtr();
    }
    *lastmsg_ptr_ = msg;
  }
  bool has_new_message(){
    return is_latest_;
  }
  nav_msgs::OdometryConstPtr* get_msg(){
    if(lastmsg_ptr_!=NULL){
      ROS_INFO("POINTER pose: (%lf, %lf, %lf)",(*lastmsg_ptr_)->pose.pose.position.x,(*lastmsg_ptr_)->pose.pose.position.y,(*lastmsg_ptr_)->pose.pose.position.z);
    }
    return lastmsg_ptr_;
  }
};
double clamp(double x, double upper, double lower)
{
    return std::min(upper, std::max(x, lower));
}
class TerrainNavigator
{
private:
  ros::Publisher cmd_pub_;
  geometry_msgs::Twist last_cmd_, twist_;
  OdomTopicHandle odom_handle_;
public:
  TerrainNavigator() :odom_handle_(OdomTopicHandle(this)){
    ros::NodeHandle nh;
    
    cmd_pub_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  }
  void publishTwist(){
    nav_msgs::OdometryConstPtr *last_odom_msg_ptr_ = odom_handle_.get_msg();
    if(last_odom_msg_ptr_!=NULL){
      nav_msgs::OdometryConstPtr last_odom_msg_ = *last_odom_msg_ptr_;
      double x=.0,y=.0,z=.0;

      x = last_odom_msg_->pose.pose.position.x;
      y = last_odom_msg_->pose.pose.position.y;
      z = last_odom_msg_->pose.pose.position.z;

      double gx,gy,dx,dy;
      dx = goalx - x;
      dy = goaly - y;
      last_odom_msg_->twist;
      ROS_INFO("odometry pose: (%lf, %lf, %lf)",x,y,z);
      geometry_msgs::Twist vel_msg;
      double angle = atan2(dy,dx);
      double dist = sqrt(pow(dx, 2) + pow(dy, 2));
      vel_msg.angular.z = clamp(0.2 * angle, .5,-.5);
      vel_msg.linear.x = clamp( 0.2 * dist,.2,-.2);

      cmd_pub_.publish(vel_msg);
    } else{
      ROS_WARN("No Odom Message Yet!");
    }
  }
};

class Listener
{
public:
ros::Subscriber sub;
    ros::NodeHandle n;
    std::string topic_;
  Listener(){
    topic_="/husky_velocity_controller/odom";
    n=ros::NodeHandle();
    sub = n.subscribe(topic_, 1, &Listener::callback, this);
  }
  void callback(const nav_msgs::Odometry::ConstPtr& msg){
    ROS_INFO("Odom recieved!!!!!!!!!!!");
  }
};

int main( int argc, char** argv )
{
    //Initializes ROS, and sets up a node
    ros::init(argc, argv, "terrain_navigation");
    ros::param::get("goalx", goalx);
    ros::param::get("goaly", goaly);

    srand(time(0));
    TerrainNavigator tnav;
    //ros::Subscriber sub = n.subscribe("/husky_velocity_controller/odom", 1, &Listener::callback, &listener);
    ros::Rate loop_rate(10);
    while(ros::ok()){
      ros::spinOnce();
      tnav.publishTwist();
      loop_rate.sleep();
    }
    //Ceates the publisher, and tells it to publish
    //to the husky_velocity_controller/cmd_vel topic, with a queue size of 100
    //ros::Publisher pub=nh.advertise<geometry_msgs::Twist>("husky_velocity_controller/cmd_vel", 100);


    //ros::NodeHandle node_;
    //topic_ = "/odometry/filtered";
    //std::string topic_ = "/husky_velocity_controller/odom";
    //ros::Subscriber subscriber_ = node_.subscribe(topic_, 1, &odomCallback);

    //Sets up the random number generator
/*

    //Sets the loop to publish at a rate of 10Hz
    ros::Rate rate(10);
  while(ros::ok()) {
    //Declares the message to be sent
    ros::spinOnce();

    tnav.publishTwist();

    //Delays until it is time to send another message
    rate.sleep();
    }*/
}
