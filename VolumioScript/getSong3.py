#!/usr/bin/python
# -*- coding: utf-8 -*-

import subprocess
import textwrap
import time
import signal
import sys
import serial
import ntplib
from time import ctime

print ("try to read Song")
ser = serial.Serial('/dev/ttyAMA0',115200,timeout=1)  # open first serial port
print ser.portstr       # check which port was really used


while True:

	# get current status and playtime
	process = subprocess.Popen('mpc', shell=True, stdout=subprocess.PIPE)
	status = process.communicate()[0]
	statusLines = status.split('\n')
	#print statusLines
	count = 0
	for statusLine in statusLines:
		if (count < 3):
			print(statusLine)
			ser.write(statusLine)
			ser.write('\r')
		count+=1
	#ser.write(status)      # write a string
	#get time
	c = ntplib.NTPClient()
	response = c.request('0.de.pool.ntp.org')
	response.strptime(response, "%d %b %y")
	print(ctime(response.tx_time))
	ser.write(ctime(response.tx_time))
	ser.write('\r')
	time.sleep(30.0)