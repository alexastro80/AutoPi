#!/usr/bin/python
import pyautogui
import busio
import digitalio
import board
import time
import os
import adafruit_mcp3xxx.mcp3008 as MCP
from adafruit_mcp3xxx.analog_in import AnalogIn
def double(case, val, count, timer, key, index):
    if(case[index] and count[index] == 1
            and timer[index] != dTimerConst and timer[index] > 0):
        val[index] = 0
        pyautogui.press(key[index])
        count[index] = 0
        timer[index] = 0
    elif(count[index] != 2):
        count[index] += 1
        timer[index] = dTimerConst
    elif(timer[index] > 0):
        timer[index] -= 1
    else:
        timer[index] = 0
        count[index] = 0

# constants
maxVal = 65536
jConstant = 5

pConstant = 100
swConstant = 300
dTimerConst = 10
rightConst = 6
leftConst = 6
upConst = 10
downConst = 20
xConst = 6 #Deadzone x
yConst = 6 #Deadzone y
leftBound = int(maxVal/xConst)
rightBound = int((xConst - 1)*maxVal/xConst)
upBound = int(maxVal/yConst)
downBound = int((yConst - 1)*maxVal/yConst)
#doubles: up, down, left, right, press
timers = [0, 0, 0, 0, 0]
counters = [0, 0, 0, 0, 0]
keys = ['b', 'b', 'v', 'n', 'h']


#Setup
spi = busio.SPI(clock=board.SCK, MISO=board.MISO, MOSI=board.MOSI)
cs = digitalio.DigitalInOut(board.CE0)
mcp = MCP.MCP3008(spi, cs)

pyautogui.FAILSAFE = False
joySW = AnalogIn(mcp, MCP.P0)
joyY = AnalogIn(mcp, MCP.P1)
joyX = AnalogIn(mcp, MCP.P2)
photo = AnalogIn(mcp, MCP.P3)

swCounter = 0
swThresh = 1
swDrag = 4
while True:
    # deadzone
    # X Axisb
    Xval = joyX.value
    if(Xval > rightBound):
        Xval = int(((Xval - rightBound)/(maxVal - rightBound))*jConstant)
        Xval = 1.5*(Xval * Xval) + Xval/2
    elif(Xval < leftBound):
        Xval = int(((leftBound-Xval)/leftBound)*jConstant)
        Xval = 1.5*(Xval * Xval) + Xval/2
        Xval = Xval * -1
    else:
        Xval = 0
#    print("Xval: "+ str(Xval))
    #Y Axis
    Yval = joyY.value
    if(Yval > downBound):
#        print(Yval)
        Yval = int(((Yval - downBound)/(maxVal - downBound))*jConstant)
        Yval = 1.5*(Yval * Yval) + Yval/2
    elif(Yval < upBound):
#        print(Yval)
        Yval = int(((upBound-Yval)/upBound)*jConstant)
        Yval = 1.5*(Yval * Yval) + Yval/2
        Yval = Yval * -1
    else:
        Yval = 0
 #   print("Yval: "+ str(Yval))
    #cases
    up = (Yval < 0)
    down = (Yval > 0)
    left = (Xval < 0)
    right = (Xval > 0)
    press = (joySW.value < swConstant)

    values = [Yval, Yval, Xval, Xval, press]
    cases = [up, down, left, right, press]


    if((up or down) ^ (left or right) ^ press):
        i = 0
        if(up):
            #print("up")
            double(cases, values, counters, timers, keys, 0)
            yVal = values[0]
        elif(down):
            #print("down")
            double(cases, values, counters, timers, keys, 1)
            yVal = values[1]
        elif(left):
            #print("left")
            double(cases, values, counters, timers, keys, 2)
            xVal = values[2]
        elif(right):
            #print("right")
            double(cases, values, counters, timers, keys, 3)
            xVal = values[3]
        elif(press):
            #print("pressed")
            double(cases, values, counters, timers, keys, 4)
            press = values[4]
    else:
        timers[0] -= 1
        timers[1] -= 1
        timers[2] -= 1
        timers[3] -= 1
        timers[4] -= 1
    lightVal = pConstant - photo.value*(pConstant/maxVal)
    if(swCounter < swDrag):
        pyautogui.moveRel(Xval,Yval, duration = 0)
    if(press):
        if(swCounter < swThresh):
            pyautogui.click()
        else:
            pyautogui.dragRel(Xval,Yval, duration = 0)
        swCounter += 1
    else:
        swCounter = 0

    #print(pyautogui.position())



    #print("Values")
    #print('Joy Switch: ', joySW.value < 1)
    #print('Joy Y-axis: ', yVal)
    #print('Joy X-axis: ', xVal)
    #print('Photo: ', lightVal)