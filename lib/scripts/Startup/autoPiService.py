# autoPiService.py
# Creates the autoPi.service file needed to run AutoPi on startup
import os
path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
path += "/Startup"
mainDir = str(path) + "/../../../"
i = mainDir.find("/home/")
j = mainDir.find("AutoPi/",i) + 7
k = mainDir.find("/", i+6)+1
workingDir = mainDir[: j]
userDir = mainDir[: k]

file = open(path + "/autoPi.service","w")
file.write("[Unit]\n")
file.write("Description=Start Auto application\n")
#file.write("After=graphical.target\n")
file.write("[Service]\n")
#file.write("Type=idle\n")
file.write("Environment=DISPLAY=:0\n")
file.write("Environment=XAUTHORITY="+userDir+".Xauthority\n")
file.write("ExecStart='" + workingDir + "bin/AutoPi'\n")
file.write("WorkingDirectory=" + workingDir + "\n") 
file.write("Restart=on-failure\n")
file.write("RestartSec=2s\n")
file.write("[Install]\n")
file.write("WantedBy=graphical.target\n")

file.close();
