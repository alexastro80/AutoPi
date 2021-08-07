import pydbus

bus = pydbus.SystemBus()

adapter = bus.get('org.bluez', '/org/bluez/hci0')
manager = bus.get('org.bluez', '/')
managedObjects = manager.GetManagedObjects()
file1 = open("connectedBluetoothDevices.txt", "w")
file2 = open("otherBluetoothDevices.txt", "w")
for path in managedObjects:
    connectState = managedObjects[path].get('org.bluez.Device1', {}).get('Connected', False)
    address = managedObjects[path].get('org.bluez.Device1', {}).get('Address')
    name = managedObjects[path].get('org.bluez.Device1', {}).get('Name')
    string = str(name) + " {" + str(address) + "}\n"
    print(string)
    file1.write(string)
file1.close()
file2.close()
