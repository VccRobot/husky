cmake_minimum_required(VERSION 2.8.3)
project(husky_navigation)

find_package(catkin REQUIRED COMPONENTS
    roslaunch
    roscpp
    std_msgs
    geometry_msgs
)

catkin_package()

roslaunch_add_file_check(launch)



install(
  DIRECTORY launch maps config
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
