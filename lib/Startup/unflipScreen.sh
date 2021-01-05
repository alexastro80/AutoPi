#!/bin/bash
# unflipScreen.sh
# unflips the screen


str=$(sudo grep -n "display_rotate=*" /boot/config.txt)
lineNum=$(echo $lineNum | sed 's/\([^:].\).*/')
if [[ -n $lineNum ]]; then
    rotateVal=$(echo $str | sed 's/\([^:].*\):\([^=].*\)=\(.*\)/\3/')
    rotateVal=$($rotateVal | 0x20000)
else
    echo "display_rotate not found"
    echo "adding display_rotate."
    #Add a line at the end.  
fi



