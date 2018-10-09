/*
 * Creating a node which showes meshes in rviz
 */

// %Tag(FULLTEXT)%
// %Tag(INCLUDES)%
#include <thread>
#include <sstream>

#include <ros/ros.h>
#include <igl/readOBJ.h>
#include <husky_cortex/CortexWorld.h>
#include <husky_cortex/Viewer.h>

using namespace husky_cortex;
 
double clamp(double x, double upper, double lower)
{
    return std::min(upper, std::max(x, lower));
}

CortexMeshmap meshmap;
CortexWorld cortexWorld;
Viewer cortexWorldViewer;

std::thread cortexWorldViewerThread;
std::thread rosThread;
Eigen::Vector3d location,next_waypoint,delta;

int simulateFreq=100, planningFreq=20;

double velocity,coverRange;
void simulate(){
    location = cortexWorld.meshmap_.location_.row(0);
    delta = next_waypoint - location;
    std::cout<<"delta1: "<<delta<<std::endl;
    delta.normalize();
    std::cout<<"delta3: "<<delta<<std::endl;
    std::cout<<"velocity: "<<velocity<<std::endl;
    std::cout<<"simulateFreq: "<<simulateFreq<<std::endl;
    delta = delta * velocity / double(simulateFreq);
    std::cout<<"delta2: "<<delta<<std::endl;
    cortexWorld.meshmap_.location_.row(0) += delta;
}
void planning(){
    std::cout<<"next_waypoint: "<<next_waypoint<<std::endl;
    std::cout<<"location: "<<location<<std::endl;
    next_waypoint = cortexWorld.get_next_waypoint();
}

void rosRunLoop(){
    //Initializes ROS, and sets up a node
    //meshmap_pub_ = node_.advertise<husky_cortex::meshmap>("/meshmap", 100);
    //reebgraph_pub_ = node_.advertise<husky_cortex::reebgraph>("/reebgraph", 100);

    //next_waypointTopic_ = "/next_waypoint";
    //subscriber_ = node_.subscribe(next_waypointTopic_, 1, &CortexWorld::next_waypointCallback, this);


    //ros::Subscriber sub = n.subscribe("/husky_velocity_controller/odom", 1, &Listener::callback, &listener);


} 

void cortexWorldInit(){
    //nh.getParam()

    double x,y,z;
    std::string meshPath,initLocation;
    
    ros::param::get("meshPath",meshPath);
    ros::param::get("initLocation", initLocation);
    std::stringstream(initLocation) >> x >> y >> z;
    ros::param::get("velocity", velocity);
    ros::param::get("coverRange",coverRange);

    ROS_INFO("meshPath: %s\n initLocation:%s\n velocity:%lf\n coverRange:%lf\n\n\n",
        meshPath.c_str(), initLocation.c_str(), velocity, coverRange);

    location = Eigen::Vector3d(x,y,z);
    next_waypoint = Eigen::Vector3d(x,y,z);
    meshmap = CortexMeshmap(meshPath, Eigen::Vector3d(x,y,z), velocity, coverRange);
    cortexWorld = CortexWorld(meshmap);
    cortexWorldViewer = Viewer(&cortexWorld);


}

int main( int argc, char** argv )
{
    ros::init(argc, argv, "CortexWorld");
    ros::NodeHandle nh;
    cortexWorldInit();

    std::cout<<ros::ok()<<std::endl;
    //rosThread = std::thread(&rosRunLoop);
    cortexWorldViewerThread = std::thread(&husky_cortex::Viewer::launch, &cortexWorldViewer);
    // /rosRunLoop();

    std::cout<<ros::ok()<<std::endl;

    std::cout<<"Oh! Ros node started!\n";
    srand(time(0));
    ros::Rate simulate_rate(simulateFreq);
    int planning_rate = simulateFreq / planningFreq;
    int counter = 0;
    while(ros::ok()){
        ros::spinOnce();
        if(counter%planning_rate==0){
            ROS_INFO("planning loop #%d",counter/planning_rate);
            planning();
        }
        counter++;

        ROS_INFO("simulate loop #%d",counter);
        simulate();
        simulate_rate.sleep();
    }
    std::cout<<ros::ok()<<std::endl;
    std::cout<<"Oh! Ros node edned!\n";
    ROS_INFO("Failed!");


    cortexWorldViewerThread.join();
    //rosThread.join();

    return 0;
}
