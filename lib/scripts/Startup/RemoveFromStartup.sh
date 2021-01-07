#!/bin/sh
# RemoveFromStartup.sh
# Removes autoPi.service from startup
sudo systemctl disable autoPi.service
sudo rm -v /lib/systemd/system/autoPi.service
sudo systemctl daemon-reload
#sudo shutdown -r +1 "REBOOT: Removing autoPi.service from startup. . ."