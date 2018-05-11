# Copyright (c) 2014 Adafruit Industries
# Author: R.Graeber
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software,	 and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


import RPi.GPIO as GPIO
from time import sleep

# Raspberry Pi gpio pin configuration
data3        = 21
data2        = 20
data1        = 16
data0        = 26
load         = 19
cp        	 = 13
flm 		 = 6
contrast 	 = 12

max_col = 320
max_row = 240


GPIO.setmode(GPIO.BCM)
GPIO.cleanup(load)
GPIO.cleanup(flm)
GPIO.cleanup(cp)
GPIO.cleanup(contrast)
GPIO.setup(contrast, GPIO.OUT, initial=GPIO.HIGH)
GPIO.setup(load, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(flm, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(cp, GPIO.OUT, initial=GPIO.LOW)
data_list = [data3,data2,data1,data0]    # add as many channels as you want!
GPIO.setup(data_list, GPIO.OUT, initial=GPIO.LOW)


#GPIO.setup(23, GPIO.OUT)
#GPIO.setup(24, GPIO.IN)

#chan_list = [11,12]    # add as many channels as you want!
#                       # you can tuples instead i.e.:
#                       #   chan_list = (11,12)
#GPIO.setup(chan_list, GPIO.OUT)
#chan_list = [11,12]                             # also works with tuples
#GPIO.output(chan_list, GPIO.LOW)                # sets all to GPIO.LOW
#GPIO.output(chan_list, (GPIO.HIGH, GPIO.LOW))   # sets first HIGH and second LOW

#fill an array
#data_array = [[1 for x in range(max_col)] for y in range(max_row)]
#data_array = [[0]*max_row for x in range(max_col)]
data_array = [[1]*max_col]*max_row


contrast_index = 0

#main loop
while True:
	#creat start impuls
	GPIO.output(flm, 1)
	GPIO.output(load , 1)
	GPIO.output(load , 0)
	GPIO.output(flm, 0)
	
	row = 0
	print "new Frame"
	for row in xrange(max_row):	
		
		#ClockPixel
		GPIO.output(load , 1)
		GPIO.output(load , 0)
		col = 0
		
		while col < max_col:
		#for col in xrange(max_col):	
			GPIO.output(cp, 1)
			GPIO.output(cp, 0)
			GPIO.output(data_list, (data_array[row][col], data_array[row][col+1], data_array[row][col+2], data_array[row][col+3]))	
			col += 4
	
	
	