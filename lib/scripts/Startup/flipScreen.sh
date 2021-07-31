#!/bin/sh

sudo grep "display_rotate=*" /boot/config.txt


lineNum=0
str=""
rotateVal=0
sudo grep -l "display_rotate=*" /boot/config.txt | $lineNum
if $lineNum != 0; then
    sudo grep "display_rotate=*" /boot/config.txt | $str
    if "$str" =~ "^(*)=(*)"; then
        rotateVal=$($rotateVal | 0x2000)
    fi
    echo   "display_rotate found on line: $lineNum"
    #replace the line.
    sudo sed -i "${lineNum}s/.*/$str/" /boot/config.txt
else
    echo "display_rotate not found"
    echo "adding display_rotate."
    #Add a line at the end.  
fi



