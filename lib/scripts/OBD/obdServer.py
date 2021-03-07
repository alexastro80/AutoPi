# This script manages the OBDII sensor for AutoPi

import argparse

import time
import os
import socket
#import pint
import sys
#dirpath = os.getcwd()
#obdPath = dirpath + "/../../../python-OBD/obd/"
#print(obdPath)
#sys.path.append(obdPath)
import obd
from obd import OBDStatus

import pint
BUFF_SIZE = 9
DEBUG = True
UDP_IP = "127.0.0.1"

parser = argparse.ArgumentParser(description='Interface with OBD module')
ureg = pint.UnitRegistry()
parser.add_argument('svrPort',
                    metavar='server_port',
                    type=int,
                    help='the local port for server communication')

def main(args):
    #setup server socket
    udp_port = args.svrPort
    svrSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    print("Binding to: " + str(udp_port))
    svrSock.bind(('localhost', udp_port))

    #Wait for message
    print("Waiting for message")
    packet, sendAddress = svrSock.recvfrom(BUFF_SIZE)
    print("Message Received!")
  #  packet = pair[0]
    if (packet is not None and len(packet) >= 4 and packet[0] == 0):
        unitSystem = packet.decode()
        unitSystem = unitSystem[1:len(unitSystem)]
        ureg.default_system = unitSystem
 #   sendAddress = pair[1]
    print("Message from " + str(sendAddress[0]) + "  " + str(sendAddress[1]))
    svrSock.settimeout(1)

    #svrSock.sendto(msg, sendAddress)

    obd.logger.setLevel(obd.logging.DEBUG)

    #Try to get a connection
    print("Connecting. . .")
    connection = obd.OBD(fast=False, timeout=30)
    print(connection.status())
    while (connection.status() != OBDStatus.CAR_CONNECTED and (not DEBUG)):
        print("Connecting. . .")
        if (not DEBUG):
            connection = obd.OBD(fast=False, timeout=30)
        print(connection.status())
        #sleep(2)
    print("Connected!")
    if (not DEBUG):
        response = connection.query(obd.commands.RPM)
    else:
        response = 3000 * ureg.meter
    if not response.is_null():
        print(response.value)
    while ((not response.is_null()) and response.value.magnitude > 300 or DEBUG):
        # Get OBD Code from application
        recvBuf = svrSock.recvfrom(BUFF_SIZE)
        packet = recvBuf[0]
        if (packet is not None and len(packet) > 0):
            # packet has been received
            sendAddress = recvBuf[1]
            packetType = int(packet[0])
            if (packetType == 2):
                codeNum = int(packet[1])
                if (not DEBUG):
                    response = connection.command[1][codeNum]
                else:
                    response = 15.56 * ureg.meter
                if (not response.is_null):
                    if (len(response) == 1):
                        #Query was for a specific value
                        response.to_base_units()
                        responseVal = double(response.value.magnitude)
                        packet = bytearray([1,struct.pack('d', responseVal)])
                        svrSock.sendto(packet)
                else:
                    packet = bytearray([1,struct.pack('d', 0.0)])
                    svrSock.sendto(packet)
        #Check if Engine was shutoff
        if (not DEBUG):
            response = connection.query(obd.commands.RPM)
        else:
           response = 3000 * ureg.meter
        if not response.is_null():
            print(response.value)
    print("Car turned off")
    # Shutdown Raspberry pi
    closeMsg = "SHUTDOWN"

    svrSock.sendto(closeMsg.encode(),sendAddress)
    time.sleep(0.2)
    svrSock.close()
    os.system('sudo shutdown now')
args = parser.parse_args()
main(args)