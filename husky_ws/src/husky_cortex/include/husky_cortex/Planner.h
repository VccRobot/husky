#pragma once
#include <husky_cortex/CortexTypes>
namespace husky_cortex{
    class Planner{
        public:
        CortexMeshmap *meshmap_;
        Planner(CortexMeshmap *meshmap):meshmap_(meshmap){}

        virtual Eigen::Vector3d next_waypoint(){

        }

    };
    class NaivePlanner{
        NaivePlanner(CortexMeshmap *meshmap):Planner(meshmap){}

        virtual Eigen::Vector3d next_waypoint(){

        }
    };


}
