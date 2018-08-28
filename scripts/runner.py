# script for start everything simultaneously, so no need for multiple terminals
# use    loam: python runall.py
# or  octomap: python runall.py loam
# outputs are put in log/

from subprocess import Popen
import signal
import time
import sys
import os
class Runner:
	def __init__(self,commands):
		self.commands=commands
	def run(self):
		commands=self.commands
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

