#!/usr/bin/python

import serial
import time

time_func = time.time
arduino = serial.Serial("/dev/ttyACM0", 9600, timeout=2)

start = time_func()

while True:
	msg = arduino.readline()
	print(msg,)
	stop = time_func()
	print(stop - start)
	start = time_func()


