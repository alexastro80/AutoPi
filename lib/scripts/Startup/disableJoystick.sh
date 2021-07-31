#!/bin/sh
# disableJoystick.sh
# Removes joystick.service from startup
sudo systemctl disable joystick.service
sudo rm -v /lib/systemd/system/joystick.service
sudo systemctl daemon-reload
# Kills joystick service, try to kill twice just incase multiple spawn for some reason.
ps axf | grep "./joystick.py" | grep -v grep | awk '{print "kill -9 " $1}' | sh
ps axf | grep "./joystick.py" | grep -v grep | awk '{print "kill -9 " $1}' | sh

#sudo shutdown -r +1 "REBOOT: Removing joystick.service from startup. . ."