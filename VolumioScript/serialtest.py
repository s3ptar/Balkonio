import serial
ser = serial.Serial('/dev/ttyAMA0',115200,timeout=1)  # open first serial port
print ser.portstr       # check which port was really used
ser.write("hello")      # write a string
ser.close()             # close port
