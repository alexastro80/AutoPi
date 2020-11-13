#!/bin/sh
# RemoveFromStartup.sh
# Removes autoPi.service from startup

sudo rm -v /lib/systemd/system/autoPi.service
sudo sudo systemctl daemon-reload
sudo shutdown -r +1 "REBOOT: Removing autoPi.service from startup. . ."