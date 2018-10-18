#pragma once
#include <Eigen/Dense>
#include <igl/serialize.h>
//#include <husky_cortex/meshmap.h>
//#include <husky_cortex/reebgraph.h>
namespace husky_cortex{
class CortexTypes : public igl::Serializable{
    public:
    virtual void toRosMsg(){};
    virtual void InitSerialization(){}
};
class CortexReebgraph: public CortexTypes{
    public:
    std::vector<std::vector<int> > graph;
    int location; // in what edge 
    Eigen::MatrixXd V_;
    Eigen::MatrixXi F_;
    Eigen::VectorXi vtype_;
    Eigen::VectorXi etype_;
    public:
    virtual void InitSerialization(){
        this->Add(V_  , "V_");
        this->Add(F_  , "F_");
        this->Add(vtype_, "vtype_");
        this->Add(etype_, "etype_");
    }
    void split(double criticalH, std::string type1, std::string type2){
        
    }
};

class CortexMeshmap: public CortexTypes{
    public:
    Eigen::Matrix<double, 1, 3> location_;
    Eigen::MatrixXd V_;
    Eigen::MatrixXi F_;
    std::vector<std::vector<int> > graph_;
    CortexReebgraph reebgraph_;
    Eigen::VectorXi vtype_;
    Eigen::VectorXi vstat_;
    Eigen::VectorXi reebi_;
    Eigen::VectorXd h_; // value of morse function
    double velocity_, coverRange_;

    std::vector<std::vector<int> >bdloops_;
    std::vector< Eigen::Vector3d > vbdP1,vbdP2;//start and end points of boundaries

    Eigen::MatrixXd V_uv_;

    public:
    virtual void InitSerialization(){
        this->Add(location_, "location_");
        this->Add(V_  , "V_");
        this->Add(F_  , "F_");
        this->Add(reebgraph_, "reebgraph_");
        this->Add(vstat_, "vstat_");
        this->Add(reebi_, "reebi_");
        this->Add(h_, "h_");
        this->Add(velocity_, "velocity_");
        this->Add(coverRange_, "coverRange_");
        this->Add(bdloops_, "bdloops_");
        this->Add(vbdP1, "vbdP1");
        this->Add(vbdP2, "vbdP2");
        this->Add(V_uv_, "V_uv_");
    }
    CortexMeshmap(){}
    CortexMeshmap(std::string meshpath, Eigen::Vector3d location, double velocity, double coverRange);
    // update meshmap and reebgraph according to current location
    void boundarySqrD(Eigen::Vector3d p, double& SqrD, int &loopi, int &vj);
    double getH();
    bool boundaryIntersect(Eigen::Vector3d s,Eigen::Vector3d d, int &wfloopi, int &wfvj);
    void updateMesh();
};

}
