#!/bin/sh
#AddToStartup.sh
#Add to Startup
DIR="$( dirname "$0" )"
echo "$DIR"
sudo rm -v /lib/systemd/system/autoPi.service
sleep 1
sudo systemctl daemon-reload
sleep 1

python $DIR/autoPiService.py
sudo cp -v $DIR/autoPi.service /lib/systemd/system/autoPi.service
sleep 1
sudo chmod 644 /lib/systemd/system/autoPi.service
sleep 1
sudo systemctl daemon-reload
sudo systemctl enable autoPi.service

