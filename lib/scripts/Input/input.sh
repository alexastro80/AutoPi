#!/bin/sh
#More logic to select input method to come.
DIR="$( dirname "$0" )"
#Assuming usage of joystick
./pythonCheck.sh pyautogui
response=$?
echo $response
if [ $response = "1" ]; then
	pip install pyautogui
	pip3 install pyautogui	
fi
./pythonCheck.sh busio
response=$?
echo $response
if [ $response = "1" ]; then
	pip install blinker
	pip3 install blinker	
fi


./pythonCheck.sh adafruit_mcp3xxx
response=$?
echo $response
if [ $response = "1" ]; then
	pip3 install adafruit-blinka   
	pip3 install adafruit-circuitpython-busdevice      
	pip3 installadafruit-circuitpython-mcp3xxx    
	pip3 install Adafruit-GPIO     
	pip3 install Adafruit-MCP3008      
	pip3 install Adafruit-PlatformDetect     
	pip3 install Adafruit-PureIO
fi
sudo python3 $DIR/joystick.py
