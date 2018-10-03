# INSTALL ROS
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
sudo apt-get update
sudo apt-get install ros-indigo-desktop-full
echo "source /opt/ros/indigo/setup.bash" >> ~/.bashrc
source ~/.bashrc
sudo rosdep init
rosdep update

# INSTALL CARTOGRAPHER

cd cartographer_ws
sudo apt-get update
sudo apt-get install -y --force-yes python-wstool python-rosdep ninja-build
source install_protobuf3.sh
rosdep install --from-path src --ignore-src --rosdistro=indigo -y
catkin_make_isolated --install --use-ninja --pkg ceres-solver cartographer cartographer_ros
cd ..

# RESTORE ENVIROMENT
sh util/rmproto3.sh
sudo apt-get purge ros-indigo-*
sudo apt-get install ros-indigo-desktop-full

# INSTALL HUSKY & VCCHUSKY
cd husky_ws
sudo apt-get install ros-indigo-husky-simulator
export HUSKY_GAZEBO_DESCRIPTION=$(rospack find husky_gazebo)/urdf/description.gazebo.xacro
sudo apt-get install ros-indigo-octomap
sudo apt-get install ros-indigo-teleop-twist-keyboard
rosdep install --from-path src --ignore-src --rosdistro=indigo -y

catkin build
