#!/bin/sh
# disableJoystick.sh
# Removes joystick.service from startup
sudo systemctl disable joystick.service
sudo rm -v /lib/systemd/system/joystick.service
sudo systemctl daemon-reload
#sudo shutdown -r +1 "REBOOT: Removing joystick.service from startup. . ."