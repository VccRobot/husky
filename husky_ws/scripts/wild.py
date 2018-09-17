# script for start everything simultaneously, so no need for multiple terminals
# use    loam: python runall.py
# or  octomap: python runall.py loam
# outputs are put in log/

from runner import *
commands=[]
commands.append("roslaunch husky_gazebo husky_wild.launch > log/husky_wild.log")
if len(sys.argv)>1 and sys.argv[1]=="loam":
    commands.append("roslaunch loam_velodyne loam_velodyne.launch > log/loam_velodyne.log")
else:
    commands.append("roslaunch octomap_server octomap_mapping.launch > log/octomapping.log")
    commands.append("roslaunch husky_viz view_robot.launch > log/view_robot.log")
    commands.append("roslaunch husky_navigation move_base_mapless_demo.launch")
commands.append("roslaunch husky_control teleop_keyboard.launch > log/teleop_keyboard.log")

r = Runner(commands)
r.run()
