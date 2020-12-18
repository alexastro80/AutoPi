#!/bin/sh
# This script starts the applicable files to start the auto application

DIR="$( dirname "$0" )"

#echo "$( dirname "$0" )"

cd "$DIR" || return ""
# execute mouse python script
# sudo ./startup/flip.sh
sudo /home/pi/openauto/bin/autoapp &
echo "Enabling Input. . ."
$DIR/Input/input.sh &
echo "Starting Head Unit. . ."
#echo "Starting OBD"
#$DIR/OBD/obd.sh &



