#!/bin/sh
# This script starts the applicable files to start the auto application

DIR="$( dirname "$0" )"

#echo "$( dirname "$0" )"

cd "$DIR" || return 1
# execute mouse python script
# sudo ./startup/flip.sh
echo "Enabling Input. . ."
#$DIR/Input/input.sh & 
#echo "Starting OBD"
#$DIR/OBD/obd.sh & 
echo "Starting Head Unit. . ."
$DIR/sizer.sh &
sudo /home/pi/openauto/bin/autoapp ||return 1 
return 0




