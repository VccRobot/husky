export VCCHUSKY_DIR=$(pwd)
source ~/.bashrc
source ${VCCHUSKY_DIR}/cartographer_ws/install_isolated/setup.bash
source ${VCCHUSKY_DIR}/husky_ws/devel/setup.bash --extend
export HUSKY_GAZEBO_DESCRIPTION=$(rospack find husky_description)/urdf/gazebo_husky_description.urdf.xacro
export HUSKY_DESCRIPTION=$(rospack find husky_description)/urdf/real_husky_description.urdf.xacro
 

