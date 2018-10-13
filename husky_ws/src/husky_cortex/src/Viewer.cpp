#include <husky_cortex/Viewer.h>
#include <husky_cortex/CortexWorld.h>
#include <iostream>

#include <igl/png/readPNG.h>

using namespace husky_cortex;
//bool husky_cortex::Viewer::pre_draw(){
CortexWorld *cworld;
Eigen::MatrixXd bdP1(3,3),bdP2(3,3);
int viewerColorScheme=0;
bool pre_draw(igl::opengl::glfw::Viewer &viewer){
    using namespace Eigen;
    using namespace std;

    CortexMeshmap &meshmap(cworld->meshmap_);

    viewer.data().set_points(meshmap.location_,RowVector3d(1,.0,.0));

    meshmap.updateMesh();

    // set mesh color
    MatrixXd C;
    igl::ColorMapType color = static_cast<igl::ColorMapType>(viewerColorScheme);
    igl::colormap(color,meshmap.vstat_,true,C);
    viewer.data().set_colors(C);

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
    //viewer_.callback_key_down = &key_down;
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

}
