# This script manages the OBDII sensor for AutoPi

import argparse
import obd
from obd import OBDStatus
import time
import os
import socket
BUFF_SIZE = 64
UDP_IP = "127.0.0.1"
parser = argparse.ArgumentParser(description='Interface with OBD module')

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
    sendAddress = pair[1]
    svrSock.sendto(msg, sendAddress)


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
    svrSock.close()
    # os.system('sudo shutdown now')
args = parser.parse_args()
main(args)