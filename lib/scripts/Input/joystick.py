#!/usr/bin/python
import pyautogui
import busio
import digitalio
import board
import time
import os
import adafruit_mcp3xxx.mcp3008 as MCP
from adafruit_mcp3xxx.analog_in import AnalogIn
 
# constants
maxVal = 65536
yAdjustment = 4000
xAdjustment = 5000
POSITIVE_BIT = 0x06

X_VAL_BITS = 0x38
Y_VAL_BITS = 0x07
SWITCH_BIT = 0X40

UPPER_LOWER = 0x04
STATE_SIZE = 2
upperVal = 0x04

global x
x = 0
#States
DRAG = 0x1F
PRESS = 0x10
LEFT = 0x08
RIGHT = 0x04
UP = 0x02
DOWN = 0x01

def getInput():
    #Convert boolean to integer and shift
    if(not switch.value):
        value = 1 << 3
    else:
        value = 0 << 3
    xVal = int(((joyX.value+xAdjustment)*6)/(maxVal)) + 1
    print("x "+ str(xVal))
    value |= (int(((joyX.value+xAdjustment)*6)/(maxVal)) + 1)
    value = value << 3
    yVal = int(((joyY.value+yAdjustment)*6)/(maxVal)) + 1
    print("y "+ str(yVal))
    value |= int(((joyY.value+yAdjustment)*6)/(maxVal)) + 1
    return value
    
def getJoyState(value):
    xVal = (value & X_VAL_BITS) >> 3
    yVal = value & Y_VAL_BITS
    if(value & SWITCH_BIT > 0):
        state = 1 << STATE_SIZE
    else: state = 0

    state |= getIndividualState(xVal)
    state = state << STATE_SIZE
    state |= getIndividualState(yVal)
    return state;
    
def getIndividualState(value):
    tempVal = (value & UPPER_LOWER)
    state = (~(value & UPPER_LOWER))
    if((value & UPPER_LOWER > 0)):
        state = 0
    else: state = 1
    state = state << 1
    if (value > UPPER_LOWER):
        state |= 1

    return state

def checkDouble(thisState, lastState):
    global x
    print(thisState)
    if(x == 0 or x == 1): return False
    elif(x >= COUNT_MAX):
        x = 0
        return False
    else: return (thisState == lastState)

#Setup
spi = busio.SPI(clock=board.SCK, MISO=board.MISO, MOSI=board.MOSI)
cs = digitalio.DigitalInOut(board.CE0)
mcp = MCP.MCP3008(spi, cs)

pyautogui.FAILSAFE = False

switch = digitalio.DigitalInOut(board.D25)
switch.direction = digitalio.Direction.INPUT
switch.pull = digitalio.Pull.UP

joyY = AnalogIn(mcp, MCP.P1)
joyX = AnalogIn(mcp, MCP.P2)
photo = AnalogIn(mcp, MCP.P0)

COUNT_MAX = 300
def main():
    thisState = 0
    lastState = 0
    global x
    doubleAction = False
    joyYScale = 2.75
    joyXScale = 2.75
    while True:
        thisVal = getInput()

        if(thisVal == 36):
            x += 1
            continue
        thisState = getJoyState(thisVal)

        doubleAction = checkDouble(thisState, lastState)
        
        #ACT
        
        if(not doubleAction):
            #Normal Action
            if((thisState & PRESS) > 0):
               pyautogui.click()
            elif((thisState & DRAG) > PRESS):
                xVal = (pow(((thisVal & X_VAL_BITS) >> 3),2)-16) * joyXScale
                yVal = (pow((thisVal & Y_VAL_BITS),2)-16) * joyYScale
                pyautogui.dragRel(xVal,yVal, duration = 0)
            else:
               xVal = (pow(((thisVal & X_VAL_BITS) >> 3),2)-16) * joyXScale
               yVal = (pow((thisVal & Y_VAL_BITS),2)-16) * joyYScale
               pyautogui.moveRel(xVal,yVal,duration = 0)
               
        else:
            if (thisState == PRESS): pyautogui.press('a')
            elif(thisState == LEFT): pyautogui.press('b')
            elif(thisState == RIGHT):pyautogui.press('c')
            elif(thisState == UP): pyautogui.press('d')
            elif(thisState == DOWN): pyautogui.press('e')
            thisState = 0
            thisVal = 0
        time.sleep(0.05)
        lastState = thisState
        print(thisState)
        x = 0
main()