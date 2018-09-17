from runner import *
commands=['roslaunch husky_gazebo husky_playpen.launch',\
          'roslaunch husky_viz view_robot.launch',\
          'roslaunch husky_navigation amcl_demo.launch',\
          ]
r = Runner(commands)
r.run()

