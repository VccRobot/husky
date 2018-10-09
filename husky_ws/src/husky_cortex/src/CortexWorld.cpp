/*
 * Creating a node which showes meshes in rviz
 */

// %Tag(FULLTEXT)%
// %Tag(INCLUDES)%
#include <husky_cortex/CortexWorld.h>

using namespace husky_cortex;

CortexWorld::CortexWorld(){}
CortexWorld::CortexWorld(CortexMeshmap meshmap)
:meshmap_(meshmap){}
CortexWorld::~CortexWorld(){}
std::vector<Eigen::Vector3d> wp_records;
Eigen::Vector3d CortexWorld::get_next_waypoint(){
    Eigen::Vector3d nxt = meshmap_.location_.row(0);
    wp_records.push_back(nxt);
    nxt += Eigen::Vector3d(0.,0.,1.);
    return nxt;
}
