DIR="$( dirname "$0" )"
$DIR/scan.sh &
sleep 5
ps axf | grep "bluetoothctl scan on" | grep -v grep | awk '{print "kill -9 " $1}' | sh
echo "Saving devices. . ."
bluetoothctl devices > $DIR/bluetoothDevices.txt
sed -i "s/^Device //" $DIR/bluetoothDevices.txt



