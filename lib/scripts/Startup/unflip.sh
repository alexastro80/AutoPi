#!/bin/bash
# unflipScreen.sh
# unflips the s

if [ ! -z ROTATE_CONSTANT ]; then
	ROTATE_CONSTANT=0x20000
fi

if sudo grep "^display_rotate=.*" /boot/config.txt; then
  	str=$(sudo grep -n "^display_rotate=*" /boot/config.txt)
	oldRotateVal=$(echo $str | sed 's/\([^:].*\):\([^=].*\)=\(.*\)/\3/')
	rotateVal=$(( $oldRotateVal & ~ROTATE_CONSTANT))
	sudo sed -ir "s/^display_rotate=.*/display_rotate=$rotateVal/" /boot/config.txt
fi



