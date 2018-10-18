#include <husky_cortex/CortexTypes.h>
#include <igl/adjacency_list.h>
#include <igl/readOBJ.h>
#include <igl/boundary_loop.h>
#include <igl/map_vertices_to_circle.h>
#include <igl/harmonic.h>
#include <igl/project_to_line_segment.h>
#include <igl/segment_segment_intersect.h>

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

    // get boundaries    
    igl::boundary_loop(F_, bdloops_);
    for(int i=0;i<bdloops_.size();i++){
        for(int j=0;j<bdloops_[i].size();j++){
            vtype_(bdloops_[i][j])=1;
        }
        std::cout<<"Loop "<<i<<"s length: "<<bdloops_[i].size()<<std::endl;
    }
    // squeeze the boundaries to start and end point list 
    for(int i=0;i<bdloops_.size();i++){
        for(int j=0;j<bdloops_[i].size();j++){
            Eigen::Vector3d pnt1(V_.row(bdloops_[i][j]));
            vbdP1.push_back(pnt1);
            int nextIndex=(j+1)%bdloops_[i].size();
            Eigen::Vector3d pnt2(V_.row(bdloops_[i][nextIndex]));
            vbdP2.push_back(pnt2);
        }
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
double getP2SegD(Eigen::Vector3d p1, Eigen::Vector3d p2, Eigen::Vector3d p){
    //typedef Eigen::Hyperplane<float,3> Line2;
    //Eigen::Vector3d a(8,2), b(9,5), c(6,6), d(5,9);

    //Line2 ac = Line2::Through(a,c), bd=Line2::Through(b,d);

    //ac.intersection(bd);
}
void CortexMeshmap::boundarySqrD(Eigen::Vector3d p, double &SqrD, int &loopi, int &vj){
    double mdist=1000000000000.0;
    int mi,mj;
    Eigen::Matrix<double,1,3> P, S, D, t, sqrD; 
    P.row(0)=p;

    for(int i=0;i<bdloops_.size();i++){
        for(int j=0;j<bdloops_[i].size();j++){
            double dist;
            int p1i=bdloops_[i][j], p2i=bdloops_[i][(j+1)%bdloops_[i].size()];
            S.row(0)=V_.row(p1i);
            D.row(0)=V_.row(p2i);

            igl::project_to_line_segment(P,S,D,t,sqrD);

            if(mdist > sqrD(0,0)){
                mdist = sqrD(0,0);
                mi=i;mj=j;
            }
        }
    }
    SqrD = mdist;
    loopi = mi;
    vj = mj;
}
double CortexMeshmap::getH(){
    return location_(0,0);
}
bool CortexMeshmap::boundaryIntersect(Eigen::Vector3d s,Eigen::Vector3d d, int &wfloopi, int &wfvj){
    for(int i=0;i<bdloops_.size();i++){
        for(int j=0;j<bdloops_[i].size();j++){
            double dist,t,u;
            int p1i=bdloops_[i][j], p2i=bdloops_[i][(j+1)%bdloops_[i].size()];
            Eigen::Vector3d p=V_.row(p1i),q=V_.row(p2i);
            Eigen::Vector3d d1=d-s, d2=q-p;

            if(igl::segments_intersect(s,d1,p,d2,t,u)==true){
                wfloopi=i;
                wfvj=j;
                return true;
            }
        }
    }
    return false;
}

