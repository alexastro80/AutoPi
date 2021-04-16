#!/bin/sh

DIR="$( dirname "$0" )"
cd $DIR

#Connect OBD
./connectOBD.sh &

#Start OBD Server
python ./obdServer.py 2222
