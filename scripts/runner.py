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
        self.ndict = dict()
        self.pdict = dict()
        for i in range(len(commands)):
            self.ndict[i]=commands[i]
            self.pdict[i]=None
    def run(self):
        if not os.path.exists('log'):
            os.mkdir('log')
        commands=self.commands
        processes = []
        for i,command in enumerate(commands):
            self.start(i)

        # interactive loop
        while(1):
            helpInfo = ("press 'e' to end all processes and this interactive program\n"
                        "press 'k' to kill all processes started above\n"
                        "or 'k INDEX' to kill a specific process whose INDEX is its command order\n"
                        "press 's' to start all process\n"
                        "or 's INDEX' to start a specific process\n"
                        "press 'r' to restart all process\n"
                        "or 'r INDEX' to restart a specific process\n"
                       )
            print helpInfo

            com = sys.stdin.readline()
            com = com.split()
            if com[0]=='e':
                for index in range(len(self.commands)):
                    self.kill(index)
                exit()
            if com[0]=='k':
                if len(com)>1:
                    index = int(com[1])
                    self.kill(index)
                if len(com)==1:
                    for index in range(len(self.commands)):
                        self.kill(index)
            if com[0]=='s':
                if len(com)>1:
                    index = int(com[1])
                    self.start(index)
                if len(com)==1:
                    for index in range(len(self.commands)):
                        self.start(index)
            if com[0]=='r':
                if len(com)>1:
                    index = int(com[1])
                    p = self.restart(index)
                if len(com)==1:
                    for index in range(len(self.commands)):
                        self.restart(index)

    def kill(self,index):
        # process killing logic
        p = self.pdict[index]
        com = self.ndict[index]
        if p is None:
            print "The {0}th command \n{1}\n is not running, you can start it by typing 'r {0}'".format(index,com)
            return
        print "killing the {0}th command \n{1}\n, whose PID is {2}".format(index,com,p.pid)
        os.killpg(os.getpgid(p.pid), signal.SIGTERM)
        self.pdict[index]=None

    def start(self,index):
        # process starting logic
        p = self.pdict[index]
        com = self.ndict[index]
        print "Starting the {0}th command \n{1}\n".format(index,com)
        if p is None:
            p = Popen([com],shell=True)
            print "Starting the {0}th command \n{1}\n, whose PID is {2}".format(index,com,p.pid)
            self.pdict[index]=p
        else:
            print "The {0}th command \n{1}\n is running, you can stop it by typing 'k {0}'".format(index,com)
            return

    def restart(self,index):
        # process restart logic
        p = self.pdict[index]
        com = self.ndict[index]
        if p is None:
            self.start(index)
            return
        else:
            print "Restarting the {0}th command \n{1}\n".format(index,com)
            self.kill(index)
            self.start(index)
