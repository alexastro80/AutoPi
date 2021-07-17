#!/bin/sh
pid=$!
DIR="$( dirname "$0" )"
cd $DIR
./connectOBD.sh $1 &
sleep 2
python ./obdServer.py 2222

