#!/usr/bin/python

import serial
import time

time_func = time.time
arduino = serial.Serial("/dev/ttyACM0", 9600, timeout=2)


# Skip the greeting messages
msg = ""
while msg != "TRIGGERED":
    msg = arduino.readline()

# Start timing
start = time_func()
while True:
    msg = arduino.readline()
    stop = time_func()
    print("{:d} ms".format(1000 * (stop - start)))
    start = time_func()

