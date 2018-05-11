#!/usr/bin/python
# -*- coding: utf-8 -*-

from time import  strftime,localtime
import serial
import time
import subprocess
import textwrap
import signal
import sys

ser = serial.Serial('/dev/ttyAMA0',19200,timeout=1)  # open first serial port


while True:
	# get current status and playtime
	str_current_time = strftime("%H:%M:%S", localtime())
	#print(str_current_time)
	#print time
	ser.write("t")
	ser.write(str_current_time)
	ser.write(",")
	
	
	
	process = subprocess.Popen('mpc', shell=True, stdout=subprocess.PIPE)
	status = process.communicate()[0]
	statusLines = status.split('\n')
	#print statusLines
	count = 0
	for statusLine in statusLines:
		if (count == 0):
			#print(statusLine)
			str_to_send = statusLine[7] + statusLine[8] + statusLine[9]
			#print(str_to_send)
			ser.write(str_to_send)
		count+=1
	time.sleep(1.0)
	ser.write('\r')
	
	

