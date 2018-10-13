#pragma once
#include <igl/opengl/glfw/Viewer.h>
#include <husky_cortex/CortexWorld.h>
namespace husky_cortex{
class Viewer{
    public:
    igl::opengl::glfw::Viewer viewer_;
    husky_cortex::CortexWorld *cortexWorld_;
    std::vector<std::string> textureFiles_;
    int colorScheme_;

    public:
    void launch(){
        viewer_.launch();
    }
    //bool pre_draw(igl::opengl::glfw::Viewer &viewer);

    Viewer():cortexWorld_(NULL){}
    Viewer(CortexWorld *cortexWorld, std::vector<std::string> textureFiles, int colorScheme);
};
}
