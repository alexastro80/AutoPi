#!/bin/bash

if [ ! -z ROTATE_CONSTANT ]; then
	ROTATE_CONSTANT=0x20000
fi

if sudo grep "^display_rotate=*" /boot/config.txt; then
	str=$(sudo grep -n "^display_rotate=*" /boot/config.txt) 
	oldRotateVal=$(echo $str |sed 's/\([^:].*\):\([^=].*\)=\(.*\)/\3/')
	rotateVal=$(( $oldRotateVal | $ROTATE_CONSTANT))
	echo $rotateVal
	sudo sed -ir "s/display_rotate=.*/display_rotate=$rotateVal/" /boot/config.txt
else
#Add to the end of file
	rotateVal=$ROTATE_CONSTANT
	sudo echo $rotateVal >> /boot/config.txt
fi

if sudo grep "^dtoverlay=" /boot/config.txt; then
	echo "Commenting out dtoverlay!"
	sudo sed -ir 's/^dtoverlay=/#dtoverlay=/' /boot/config.txt
fi

