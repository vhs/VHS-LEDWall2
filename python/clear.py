import socket
import sys
import time
import random
import base64
import SocketServer, serial
import os
ser = serial.Serial('/dev/ttyACM0', 256000, timeout=1, writeTimeout=1)
###ser = serial.Serial('/dev/ttyACM0', 115200, timeout=5, writeTimeout=5)
print ser
###logger = open( "test-ser.log", 'w' );
###datalog = open( "data.log", 'w' );

HOST, PORT = "localhost", 9999
FIXTURES = 768
CHANNELS = (FIXTURES*3)

print "FIXTURES:",FIXTURES

while True:
	try:
		ret = ser.write(chr(0))
		res = ser.readline().strip()
		print "[" + str(res) + "]"
	except serial.SerialException as msg:
		print msg
	finally:
		if res == '*':
			while True:
				print '.'
				ret = ser.write(chr(0))
				res = ser.read().strip()
				if res == '#':
					break
	ser.flushOutput()
