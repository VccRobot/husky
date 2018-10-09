#pragma once
#include <Eigen/Dense>
//#include <husky_cortex/meshmap.h>
//#include <husky_cortex/reebgraph.h>
namespace husky_cortex{
class CortexTypes{
    public:
    virtual void toRosMsg(){};
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

    public:
    CortexMeshmap(){}
    CortexMeshmap(std::string meshpath, Eigen::Vector3d location, double velocity, double coverRange);
    // update meshmap and reebgraph according to current location
    void updateMesh();
};

}
