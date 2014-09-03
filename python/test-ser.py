import socket
import sys
import time
import random
import base64
import SocketServer, serial
import os
ser = serial.Serial('/dev/ttyACM0', 250000, timeout=5, writeTimeout=5)
###ser = serial.Serial('/dev/ttyACM0', 115200, timeout=5, writeTimeout=5)
print ser
###logger = open( "test-ser.log", 'w' );
###datalog = open( "data.log", 'w' );

HOST, PORT = "localhost", 9999
FIXTURES = 768
CHANNELS = (FIXTURES*3)

print "FIXTURES:",FIXTURES

canvas = []

for c in range(0,CHANNELS):
	canvas.append( 0 )

###sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
###sock.connect((HOST, PORT))

while True:
	for c in range(0,CHANNELS):
		if canvas[c] > 0:
			canvas[c] = canvas[c] - 1

	pixel = random.randint(0,(FIXTURES-1))
	r=pixel*3
	g=r+1
	b=g+1
	canvas[r] = 255 ###random.randint(0,255)
	canvas[g] = 255 ###random.randint(0,255)
	canvas[b] = 255 ###random.randint(0,255)

	data = ''
	for c in range(0,CHANNELS):
		data = data + chr(canvas[c])

	###datalog.write(data)

	try:
		res = ser.readline().strip()
		print res
	except serial.SerialException as msg:
		print msg
	finally:
		if res == '*':
			print "Writing data..."
			###for i in range(0,len(data)):
				###ret = ser.write(data[i])
			ret = ser.write(data)
			print ret
	
			res = ser.readline().strip()
			print res

			###time.sleep(0.025)
			ser.flushOutput()

		###time.sleep(0.005)

###sock.close()
