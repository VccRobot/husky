#include <husky_cortex/Viewer.h>
#include <husky_cortex/CortexWorld.h>
#include <iostream>

#include <igl/png/readPNG.h>

using namespace husky_cortex;
//bool husky_cortex::Viewer::pre_draw(){
CortexWorld *cworld;
Eigen::MatrixXd bdP1(3,3),bdP2(3,3);
Eigen::MatrixXd crits, scans;
int viewerColorScheme=0;
Eigen::Matrix<double,1,3> intersection;
int lastSeenIndex=0, lastScanIndex=0;

bool pre_draw(igl::opengl::glfw::Viewer &viewer){
    using namespace Eigen;
    using namespace std;

    CortexMeshmap &meshmap(cworld->meshmap_);

    viewer.data().set_points(meshmap.location_,RowVector3d(1,.0,.0));
//    int intersectionI = meshmap.bdloops_[cworld->wfloopi_][cworld->wfvj_];
//    intersection.row(0)=meshmap.V_.row(intersectionI);
//    viewer.data().add_points(intersection,RowVector3d(0, 1.,.0));

    meshmap.updateMesh();

    // set mesh color
    MatrixXd C;
    igl::ColorMapType color = static_cast<igl::ColorMapType>(viewerColorScheme);
    igl::colormap(color,meshmap.vstat_,true,C);
    viewer.data().set_colors(C);

    // add criticle points visualization
    std::vector<int> seenCrits;
//    for(int i=0;i<meshmap.criticalPoints_.size();i++)
    int critsNum = meshmap.criticalPoints_.size();
    if( critsNum >0){
        crits.resize( critsNum,3);
        for(int i=0;i<critsNum;i++){
            crits.row(i) = meshmap.V_.row(meshmap.criticalPoints_[i]);
        }
        viewer.data().add_points(crits, RowVector3d(0,1.,.0));
    }

    int scansNum = meshmap.scanPoints_.size();
    if( scansNum >0){
        scans.resize( scansNum,3);
        for(int i=0;i<scansNum;i++){
            scans.row(i) = meshmap.scanPoints_[i];
        }
        viewer.data().add_points(scans, RowVector3d(0.,0.,1.));
    }

    // add boundary edges visualization
    std::vector< Eigen::Vector3d > vbdP1,vbdP2;
    for(int i=0;i<meshmap.bdloops_.size();i++){
        for(int j=0;j<meshmap.bdloops_[i].size();j++){
            Eigen::Vector3d pnt1(meshmap.V_.row(meshmap.bdloops_[i][j]));
            vbdP1.push_back(pnt1);
            int nextIndex=(j+1)%meshmap.bdloops_[i].size();
            Eigen::Vector3d pnt2(meshmap.V_.row(meshmap.bdloops_[i][nextIndex]));
            vbdP2.push_back(pnt2);
        }
    }
    bdP1.resize(vbdP1.size(),3);
    bdP2.resize(vbdP2.size(),3);

    for(int i=0;i<vbdP1.size();i++){
            bdP1.row(i)=vbdP1[i];
            bdP2.row(i)=vbdP2[i];
    }
    viewer.data().add_edges(bdP1,bdP2, RowVector3d(1,0,0)); 
    return false;
}
unsigned char UP_KEY(9),DOWN_KEY(8),LEFT_KEY(7),RIGHT_KEY(6),PAGEUP_KEY(10),PAGEDOWN_KEY(11);
unsigned char lastKey(0);
bool key_up(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifer){
    if(lastKey==UP_KEY || lastKey==DOWN_KEY || lastKey==LEFT_KEY || lastKey==DOWN_KEY){
        cworld->meshmap_.nextWayPoint_ = Eigen::Vector3d(cworld->meshmap_.location_.row(0));
    }
    return false;
}
bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifer){
    Eigen::Vector3d dir(0.,0.,0.);
    std::cout<<"Key: "<<key<<" "<<(unsigned int)key<<std::endl;
    if(key==UP_KEY){
        dir = Eigen::Vector3d(0.,0.,-1000.);
    }else if(key==LEFT_KEY){
        dir = Eigen::Vector3d(-1000.,0.,0.);
    }else if(key==DOWN_KEY){
        dir = Eigen::Vector3d(0.,0.,1000.);        
    }else if(key==RIGHT_KEY){
        dir = Eigen::Vector3d(1000.,0.,0.);
    }else if(key==PAGEUP_KEY){
        cworld->meshmap_.velocity_ *= 1.2;
    }else if(key==PAGEDOWN_KEY){
        cworld->meshmap_.velocity_ /= 1.2;
    }

    if(dir.norm()>1.){
        cworld->meshmap_.nextWayPoint_ = Eigen::Vector3d(cworld->meshmap_.location_.row(0)) + dir;
    }
    return false;
}

igl::opengl::glfw::imgui::ImGuiMenu menu;
husky_cortex::Viewer::Viewer(CortexWorld *cortexWorld, std::vector<std::string> textureFiles, int colorScheme)
:cortexWorld_(cortexWorld), textureFiles_(textureFiles), colorScheme_(colorScheme){
    cworld = cortexWorld;
    viewer_.data().clear();
    viewer_.data().set_mesh(cortexWorld_->meshmap_.V_, cortexWorld_->meshmap_.F_);
 
    viewer_.data().show_lines = false;
    //viewer_.data().set_colors(C);
    //viewer_.core.trackball_angle = Eigen::Quaternionf(sqrt(2.0),0,sqrt(2.0),0);
    //viewer_.core.trackball_angle.normalize();
    viewer_.callback_pre_draw = &(pre_draw);
    viewer_.callback_key_down = &key_down;
    viewer_.callback_key_up = &key_up;
    viewer_.core.is_animating = true;
    viewer_.core.animation_max_fps = 30.;
    
    //color
    viewerColorScheme = colorScheme_;
    
    //load texture
    viewer_.data().set_uv(cortexWorld->meshmap_.V_uv_);
    viewer_.data().show_texture = true;
    Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> R,G,B,A;
    igl::png::readPNG(textureFiles_[0],R,G,B,A);
    viewer_.data().set_texture(R,G,B);


    viewer_.plugins.push_back(&menu);

}
