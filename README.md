# Husky in The Wild

Ongoing development of exploration robot navigation in unknown outdoor environment.

## Installation
The code is build on top of [ROS - Indigo](http://www.ros.org/) and simulate Husky robot using [Gazebo](gazebosim.org). The code has been tested only on Ubuntu 14.04. Please follow the following steps in order to install the package along with its dependences:

- Install ROS-Indigo full desktop ([guide](http://wiki.ros.org/indigo/Installation/Ubuntu])).

- Install Husky official package ([guide](http://wiki.ros.org/husky_gazebo/Tutorials/Simulating%20Husky))

- Install the Octomap package that can be optionally used for mapping ([guide](http://wiki.ros.org/octomap))

- Install the Teleop Twist Keyboard package to control Husky using keyboard:
```
sudo apt-get install ros-indigo-teleop-twist-keyboard
```

- Install Husky other dependences. Assuming Husky package is cloned under `$HUSKY_DIR$`:
```
cd $HUSKY_DIR$
rosdep install --from-path src --ignore-src --rosdistro=indigo -y
```

- Install our Husky package:
```
cd $HUSKY_DIR$
catkin_make
```


- The Gazebo version shipped with ROS-Indigo has known issue of being unable to download Gazebo models from Gazebo online repository. In order to avoid errors that might come up due to this issue, download and extract Gazebo models ([here [ 231MB] ](https://bitbucket.org/osrf/gazebo_models/downloads/)) and place them under `~/.gazebo/models/`



## Running
Before you should run 'source devel/setup.bash'in project root in order to setup the environment.

The code builds each useful functionality in a separate package for better modularity. In order to run the simulated Husky, you can run every components at once or step by step.

Script for starting everything at once, so no need for multiple terminals.
use octomap: 'python scripts/runall.py'
or     loam: 'python scripts/runall.py loam'

Step by step:
**Basic Husky:**

```
roslaunch husky_gazebo husky_playpen.launch
```
Other launch files can be used for different other outdoor environments e.g., `husky_wild.launch`.

 **Mapping:**

 In a different terminal, run
```
roslaunch octomap_server octomap_mapping.launch
```

This is for mapping using `Octomap`. Alternatively, SLAM LOAM can be used as follows
```
roslaunch loam_velodyne loam_velodyne.launch
```



 **Visualization:**

With mapping using SLAM LOAM, it launches its own visualization (`rviz`).

For mapping `Octomap` or to just visualize the pointcloud from Husky, run in a different terminal  
```
roslaunch husky_viz view_robot.launch
```
On the left panel, user can choose to either visualize the instant pointcloud or the constructed map by changing the `Topic` of `Sensing->PointCloud2`.

**Controlling:**

To control Husky using the keyboard, run in a different terminal
```
roslaunch husky_control teleop_keyboard.launch
```


## Parameter Hacking

**TODO**
