from runner import *
commands=['roslaunch husky_gazebo husky_playpen.launch',\
          'roslaunch husky_control teleop_keyboard.launch',\
          ]
r = Runner(commands)
r.run()

