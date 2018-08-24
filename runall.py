# script for start everything simultaneously, so no need for multiple terminals
# use    loam: python runall.py
# or  octomap: python runall.py loam
# outputs are put in log/

from subprocess import Popen
import signal
import time
import sys
import os
print os.getcwd()
commands=[]
commands.append("roslaunch husky_gazebo husky_playpen.launch > log/husky_gazebo.log")
if len(sys.argv)>1 and sys.argv[1]=="loam":
    commands.append("roslaunch loam_velodyne loam_velodyne.launch > log/loam_velodyne.log")
else:
    commands.append("roslaunch octomap_server octomap_mapping.launch > log/octomapping.log")
    commands.append("roslaunch husky_viz view_robot.launch > log/view_robot.log")
commands.append("roslaunch husky_control teleop_keyboard.launch > log/teleop_keyboard.log")
processes = []
for command in commands:
    print "Start running command: "+command
    p = Popen([command],shell=True)
    processes.append(p)
    time.sleep(3)

while(1):
    print "press q to kill all processes started above"
    com = sys.stdin.read(1)
    if com=='q':
        for p in processes:
            print p
            os.killpg(os.getpgid(p.pid), signal.SIGTERM)
        exit()

