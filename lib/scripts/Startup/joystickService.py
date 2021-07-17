# joystickService.py
# Creates the joystick.service file needed to run joystick on startup.
import os
path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
path += "/Startup"
mainDir = str(path) + "/../../../"
i = mainDir.find("/home/")
j = mainDir.find("AutoPi/",i) + 7
k = mainDir.find("/", i+6)+1
workingDir = mainDir[: j]
userDir = mainDir[: k]

file = open(path + "/joystick.service","w")
file.write("[Unit]\n")
file.write("Description=Joystick service\n")
file.write("After=multi-user.target\n")
file.write("[Service]\n")
#file.write("Type=idle\n")
file.write("ExecStart=" + workingDir + "lib/scripts/Input/input.sh'\n")
file.write("WorkingDirectory=" + workingDir + "\n") 
file.write("Restart=on-failure\n")
file.write("RestartSec=2s\n")
file.write("[Install]\n")
file.write("WantedBy=multi-user.target\n")

file.close();