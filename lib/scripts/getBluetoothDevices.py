import pydbus

bus = pydbus.SystemBus()

adapter = bus.get('org.bluez', '/org/bluez/hci0')
manager = bus.get('org.bluez', '/')
managedObjects = manager.GetManagedObjects()
for path in managedObjects:
    connectState = managedObjects[path].get('org.bluez.Device1', {}).get('Connected', False)
    if connectState:
        address = managedObjects[path].get('org.bluez.Device1', {}).get('Address')
        name = managedObjects[path].get('org.bluez.Device1', {}).get('Name')
        print(f'Device {name} [{address}] is connected')
            
