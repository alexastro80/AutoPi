# service.py
# Creates the autoPi.service file needed to run AutoPi on startup
import os
path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
path += "/Startup"
mainDir = str(path) + "/../../../"
i = mainDir.find("/home/")
j = mainDir.find("/", i + 6) + 1
userDir = mainDir[: j]

file = open(path + "/autoPi.service","w")
file.write("[Unit]\n")
file.write("Description=Start Auto application\n")
file.write("[Service]\n")
file.write("Environment=DISPLAY=:0\n")
file.write("Environment=XAUTHORITY="+userDir+".Xauthority\n")
file.write("ExecStart='" + mainDir + "bin/AutoPi'\n")
file.write("KillMode=mixed\n")
file.write("Restart=on-failure\n")
file.write("RestartSec=2s\n")
file.write("[Install]\n")
file.write("WantedBy=graphical.target\n")

file.close();