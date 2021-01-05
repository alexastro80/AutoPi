#!/bin/bash


str=$(sudo grep -n "^display_rotate=*" /boot/config.txt)
lineNum=$(echo $str | sed 's/\([^:]*\).*/\1/')
echo "$lineNum"
if [[ -n $lineNum ]]; then
	rotateVal=$(echo $str | sed 's/\([^:].*\):\([^=].*\)=\(.*\)/\3/')
	rotateVal=$(( $rotateVal | 131072))
	echo $rotateVal
else
	sudo echo "display_rotate=131072" | sudo tee -a sudo /boot/config.txt
fi

reboot 

