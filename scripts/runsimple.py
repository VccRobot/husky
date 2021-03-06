# script for start everything simultaneously, so no need for multiple terminals
# use    loam: python runall.py
# or  octomap: python runall.py loam
# outputs are put in log/

from runner import *
commands=[]
commands.append("roslaunch husky_gazebo husky_playpen.launch > log/husky_gazebo.log")
commands.append("roslaunch husky_control teleop_keyboard.launch > log/teleop_keyboard.log")

r = Runner(commands)
r.run()
