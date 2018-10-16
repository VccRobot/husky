#pragma once
#include <string>

//#include <ros/ros.h>
//#include <tf/transform_datatypes.h>
//#include <geometry_msgs/Quaternion.h>
//#include <geometry_msgs/Vector3.h>
//#include <geometry_msgs/Twist.h>
//#include <nav_msgs/Odometry.h>
//#include <husky_cortex/reebgraph.h>
//#include <husky_cortex/meshmap.h>

#include <igl/serialize.h>
#include <husky_cortex/CortexTypes.h>

namespace husky_cortex{

class Planner;

double clamp(double x, double upper, double lower);
class CortexWorld : public igl::Serializable{
    public:
    // ROS MODULE
    //ros::Publisher meshmap_pub_, reebgraph_pub_;
    //ros::NodeHandle node_;
    //geometry_msgs::Twist last_cmd_, twist_;
    //std::string next_waypointTopic_;
    //ros::Subscriber subscriber_;

    husky_cortex::CortexMeshmap meshmap_;
    
    // planner variables
    // planMode_ has value 0 or 1, the other two hs value -1 or 1
    int planMode_, tanDir_, normDir_;
    Eigen::Vector3d tanDelta_,normDelta_;
    double bndDThreshold_;

    double lastTanH_, normDThreshold_;

    virtual void InitSerialization(){
        this->Add(meshmap_, "meshmap_");
        this->Add(planMode_  , "planMode_");
        this->Add(normDir_  , "normDir_");
        this->Add(tanDelta_, "tanDelta_");
        this->Add(normDelta_, "normDelta_");
        this->Add(bndDThreshold_, "bndDThreshold_");
        this->Add(lastTanH_, "lastTanH_");
        this->Add(normDThreshold_, "normDThreshold_");
    }
    CortexWorld();
    CortexWorld(CortexMeshmap meshmap, double bndDThreshold, double normDThreshold);
    ~CortexWorld();
    //void next_waypointCallback(const geometry_msgs::PointConstPtr& msg){
    //   ROS_INFO("Next Waypoint: (%lf, %lf, %lf)",msg->x,msg->y,msg->z);
    //}

    void publishMap(){
        
    }
    void publishLocation(){
        
    }

    virtual Eigen::Vector3d get_next_waypoint();
    
};

}
