cmake_minimum_required(VERSION 2.8.3)
project(husky_gazebo)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
    roslaunch
    roscpp
    visualization_msgs
)
catkin_package()
catkin_add_env_hooks(env_hooks
                     SHELLS sh DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/env-hooks)
roslaunch_add_file_check(launch)

add_executable(show_mesh src/show_mesh.cpp)
target_link_libraries(show_mesh ${catkin_LIBRARIES})

install(
  DIRECTORY launch worlds urdf
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
