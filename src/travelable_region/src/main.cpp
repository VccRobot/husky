#include "GroundExtraction.h"

int main(int argc, char** argv){

  ros::init(argc, argv, "path_finder");
  ros::NodeHandle node;
  ros::NodeHandle privateNode("~");
 
  GroundExtraction Pathfinder(node,privateNode);

  ros::spin();

  return 0;
}

