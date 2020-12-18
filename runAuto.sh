#!/bin/sh
# This script starts the applicable files to start the auto application

DIR="$( dirname "$0" )"

#echo "$( dirname "$0" )"

cd "$DIR" || return ""
# execute mouse python script
# sudo ./startup/flip.sh
echo "Enabling Input. . ."
sudo ./Input/input.sh &
echo "Starting Head Unit. . ."
sudo /home/pi/openauto/bin/autoapp &
#echo "Starting OBD"
#sudo ./OBD/obd.sh &



