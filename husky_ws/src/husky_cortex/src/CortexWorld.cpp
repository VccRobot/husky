/*
 * Creating a node which showes meshes in rviz
 */

// %Tag(FULLTEXT)%
// %Tag(INCLUDES)%
#include <husky_cortex/CortexWorld.h>
#include <iostream>

using namespace husky_cortex;

CortexWorld::CortexWorld(){}
CortexWorld::CortexWorld(CortexMeshmap meshmap, double bndDThreshold, double normDThreshold)
:meshmap_(meshmap), bndDThreshold_(bndDThreshold), normDThreshold_(normDThreshold),
wfloopi_(0),wfvj_(0){
    planMode_=0;
    tanDir_ = 1;
    normDir_ = 1;
    tanDelta_ = tanDir_ * Eigen::Vector3d(0.,0.,1.);
    normDelta_ = normDir_ * Eigen::Vector3d(1.,0.,0.);
}
CortexWorld::~CortexWorld(){}
std::vector<Eigen::Vector3d> wp_records;

Eigen::Vector3d CortexWorld::get_next_waypoint(){
    Eigen::Vector3d current = meshmap_.location_.row(0),nxt=current;
    double bndSqrD;int loopi,vj;
    loopi = wfloopi_; vj=wfvj_;
    meshmap_.boundarySqrD(meshmap_.location_, bndSqrD, loopi, vj);


    // moving
    if(planMode_==0){

        bool intersect= meshmap_.boundaryIntersect(current, current+tanDelta_*bndDThreshold_, loopi, vj);

        // if is too close to the wall then switching mode
        if(intersect){
            planMode_=1;
            lastTanH_ = meshmap_.getH();
            wfloopi_ = loopi;
            wfvj_ = vj;
            std::cout<<"INTERSECTION INFO!!!!!!!!!!!!!!!!!!!\n";
            std::cout<<"LOCATION:\n";
            std::cout<< current<<"\n\n"<<current+tanDelta_ <<"\n"<<meshmap_.bdloops_[loopi][vj]<<"\n";
            std::cout<<"INTERSECTED SEGMENTS!!!!!!!!!!!!!!!!!!!\n";
            Eigen::Vector3d start = meshmap_.V_.row(meshmap_.bdloops_[loopi][vj]);
            Eigen::Vector3d end = meshmap_.V_.row(meshmap_.bdloops_[loopi][(vj+1)%meshmap_.bdloops_[loopi].size()]);
            std::cout<<start << "\n\n"<< end << "\n";
        }else{
            nxt = current + tanDelta_*bndDThreshold_;
        }
    }
    else if(planMode_==1){ // wall following
        double currentTanH = meshmap_.getH(), hdiff=std::abs(currentTanH - lastTanH_);
        if( hdiff > normDThreshold_){
            planMode_ = 0;
            tanDelta_(2) = -tanDelta_(2);
            tanDir_ = -tanDir_;
        }else{
            int nv = meshmap_.bdloops_[loopi].size();
            int i1=meshmap_.bdloops_[loopi][vj], 
                i2=meshmap_.bdloops_[loopi][(vj+1)%nv],
                i3;
            Eigen::MatrixXd &V(meshmap_.V_);
            Eigen::Vector3d p1,p2,p3,pgoal;
            p1=V.row(i1),p2=V.row(i2);
            if(p1(0) <= p2(0)){
                i3 =meshmap_.bdloops_[loopi][(vj+2)%nv];
                p3=V.row(i3);
                pgoal = p3;
                wfvj_+=1;
            }
            else{
                i2=meshmap_.bdloops_[loopi][(vj-1)%nv];
                p2 = V.row(i2);
                i3=i1;
                p3 = V.row(i3);
                pgoal = p2;
                wfvj_-=1;
            }
            Eigen::Vector3d vec=p3-p2;
            Eigen::Vector3d vecNormal=Eigen::Vector3d(0., 1., 0.).cross(vec);
            vecNormal.normalize();
            Eigen::Vector3d next = pgoal + vecNormal * (bndDThreshold_+1.);
            if((next(0)-meshmap_.location_(0,0))*normDir_ > 0){
                nxt = next - current;
                nxt.normalize();
                nxt = current + nxt * 20.;
            }else{
                planMode_ = 0;
            }
            //nxt += normDelta_;
        }
    }

    wp_records.push_back(meshmap_.location_);
    return nxt;
}
