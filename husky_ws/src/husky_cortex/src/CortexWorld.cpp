/*
 * Creating a node which showes meshes in rviz
 */

// %Tag(FULLTEXT)%
// %Tag(INCLUDES)%
#include <husky_cortex/CortexWorld.h>

using namespace husky_cortex;

CortexWorld::CortexWorld(){}
CortexWorld::CortexWorld(CortexMeshmap meshmap)
:meshmap_(meshmap){
    planMode_=0;
    tanDir_ = 1;
    normDir_ = 1;
    tanDelta_ = tanDir_ * Eigen::Vector3d(0.,0.,1.);
    normDelta_ = normDir_ * Eigen::Vector3d(1.,0.,0.);
}
CortexWorld::~CortexWorld(){}
std::vector<Eigen::Vector3d> wp_records;

Eigen::Vector3d CortexWorld::get_next_waypoint(){
    Eigen::Vector3d nxt = meshmap_.location_.row(0);

    // moving
    if(planMode_==0){
        double bndD = meshmap_.boundaryDist(meshmap_.location_);

        // if is too close to the wall then switching mode
        if(bndD<bndDThreshold_){
            planMode_=1;
        }else{
            nxt += tanDelta_;
        }
    }
    else if(planMode_==1){ // wall following
        double currentTanH = meshmap_.getH();
        if(std::abs(currentTanH - lastTanH_) > normDThreshold_){
            planMode_ = 0;
            tanDelta_ *= -1;
            tanDir_ *= -1;
        }else{
            nxt += normDelta_;
        }
    }

    wp_records.push_back(nxt);
    nxt += Eigen::Vector3d(0.,0.,1.);
    return nxt;
}
