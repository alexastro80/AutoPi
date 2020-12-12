# This script manages the OBDII sensor for AutoPi


import obd
from obd import OBDStatus
import time
import os

obd.logger.setLevel(obd.logging.DEBUG)

#Try to get a connection
print("Connecting. . .")
connection = obd.OBD(fast=False, timeout=30)
print(connection.status())
while (connection.status() != OBDStatus.CAR_CONNECTED):
    print("Connecting. . .")
    connection = obd.OBD(fast=False, timeout=30)
    print(connection.status())
    #sleep(2)
print("Connected!")
response = connection.query(obd.commands.RPM)
if not response.is_null():
    print(r.value)
while (response.value.magnitude > 300):
    response = connection.query(obd.commands.RPM)
    if not response.is_null():
        print(r.value)
print("Car turned off")
# Shutdown Raspberry pi
# os.system('sudo shutdown now')