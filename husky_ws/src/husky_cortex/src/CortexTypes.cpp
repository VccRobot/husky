#include <husky_cortex/CortexTypes.h>
#include <igl/adjacency_list.h>
#include <igl/readOBJ.h>
#include <igl/boundary_loop.h>
#include <igl/map_vertices_to_circle.h>
#include <igl/harmonic.h>

#include <iostream>
using namespace husky_cortex;

CortexMeshmap::CortexMeshmap(std::string meshpath, Eigen::Vector3d location, double velocity, double coverRange)
:velocity_(velocity),coverRange_(coverRange){
    igl::readOBJ(meshpath, V_, F_);

    igl::adjacency_list(F_, graph_);
    int vn = V_.rows();
    location_.row(0) = location;
    vstat_= Eigen::VectorXi::Constant(vn,0);
    vtype_= Eigen::VectorXi::Constant(vn,0);
    reebi_= Eigen::VectorXi::Constant(vn,0);
    h_= Eigen::VectorXd::Constant(vn,.0);

    
    igl::boundary_loop(F_, bdloops_);
    for(int i=0;i<bdloops_.size();i++){
        for(int j=0;j<bdloops_[i].size();j++){
            vtype_(bdloops_[i][j])=1;
        }
        std::cout<<"Loop "<<i<<"s length: "<<bdloops_[i].size()<<std::endl;
    }

    // compute UV coordinates(parametrization)
      // Map the boundary to a circle, preserving edge proportions
    Eigen::MatrixXd bnd_uv;
    Eigen::VectorXi bnd; // compue bnd loops again, this time save as Eigen Vector
    igl::boundary_loop(F_, bnd);
    igl::map_vertices_to_circle(V_, bnd, bnd_uv);
    // Harmonic parametrization for the internal vertices
    igl::harmonic(V_,F_,bnd,bnd_uv,1,V_uv_);
    //V_uv_*=10.;

    updateMesh();
}
void CortexMeshmap::updateMesh(){
    for(int i=0;i<V_.rows();i++){
        if( (location_.row(0) - V_.row(i)).norm()<coverRange_ ){
            // checking old critical points
            // TODO
            if(vstat_(i)==1){
                continue;
            }
            vstat_(i)=1;
            continue;
            // checking new critical points

            double maxh=-10000000.,minh=10000000.;
            for(int adji=0;adji<graph_[i].size();adji++){
                int adjv = graph_[i][adji];
                if(vstat_[adjv]==1 && vtype_[adjv]>=1){
                    maxh=std::max(maxh,h_[adjv]);
                    minh=std::min(minh,h_[adjv]);
                }
            }

            // encountered new split point
            if(h_[i]< minh){
                reebgraph_.split(h_[i], "lower", "lower");
            }
            // encountered new merge point
            if(h_[i] > maxh){
                reebgraph_.split(h_[i], "lower", "upper");
            }
        }
    }
}
