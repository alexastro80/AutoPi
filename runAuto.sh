#!/bin/sh
# This script starts the applicable files to start the auto application

DIR="$( dirname "$0" )"

#echo "$( dirname "$0" )"

cd "$DIR"
# execute mouse python script
# sudo ./startup/flip.sh

sudo python3 AlexAuto.py &

#sudo python OBD.py &



