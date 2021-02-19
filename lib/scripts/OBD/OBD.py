# This script manages the OBDII sensor for AutoPi

import argparse
import obd
from obd import OBDStatus
import time
import os
import socket
import pint
BUFF_SIZE = 64

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
    svrSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    svrSock.bind((UDP_IP, udp_port))
    #Wait for message
    pair = svrSock.recvfrom(BUFF_SIZE)
    msg = pair[0].decode()
    if len(msg) == 3:
        ureg.default_system = msg
    sendAddress = pair[1]
    svrSock.settimeout(1)
    #svrSock.sendto(msg, sendAddress)


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
        print(response.value)
    while ((not response.is_null()) and response.value.magnitude > 300):
        # Get OBD Code from application
        recvBuf = svrSock.recvfrom(BUFF_SIZE)
        code = recvBuf[0].decode()
        if ((code is not None) and (len(code) > 0)):
            #Code has been received
            sendAddress = recvBuf[1]
            codeNum = int(code)
            response = connection.command[1][codeNum]
            if (not response.is_null):
                if (len(response) == 1):
                    #Query was for a specific value
                    response.to_base_units()
                    responseString = str(response.value.magnitude)
                    svrSock.sendto(responseString.encode())
            else:
                responseString = str(-1) 
                svrSock.sendto(responseString.encode())
        #Check if Engine was shutoff
        response = connection.query(obd.commands.RPM)
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