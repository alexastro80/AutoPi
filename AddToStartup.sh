#!/bin/sh
#AddToStartup.sh
#Add to Startup
sudo cp ./Startup/autoPi.service /lib/systemd/system/autoPi.service

sudo systemctl daemon-reload
sudo systemctl enable autoPi.service

sudo shutdown -r +0.2 "REBOOT: Adding autoPi.service to startup. . ."

