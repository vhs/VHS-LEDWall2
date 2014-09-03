import socket
import sys
import time
import random
import base64
import SocketServer, serial
###ser = serial.Serial('/dev/ttyACM0', 250000)
ser = serial.Serial('/dev/ttyACM0', 115200)

HOST, PORT = "localhost", 9999
FIXTURES = 768
CHANNELS = (FIXTURES*3)

print "FIXTURES:",FIXTURES

canvas = []

for c in range(0,CHANNELS):
	canvas.append( 0 )

###sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
###sock.connect((HOST, PORT))

###for f in range( 0, (40*900) ):
while True:
	for c in range(0,CHANNELS):
		if canvas[c] > 0:
			canvas[c] = canvas[c] - 1

	random_pixel = random.randint( 1, (FIXTURES-1) )

	r_pixel = (random_pixel*3)
	g_pixel = r_pixel + 1
	b_pixel = g_pixel + 1
	canvas[r_pixel] = random.randint(0,255)
	canvas[g_pixel] = random.randint(0,255)
	canvas[b_pixel] = random.randint(0,255)

	data = ''
	for c in range(0,CHANNELS):
		###data = data + chr(canvas[c])
		data = data + chr(random.randint(0,255))

	res = ser.readline().strip()
	print res

	if res == '*':
		print "Writing data..."
		for i in range(0,(len(data)/32)):
			print "!/" + str(i*32)
			ser.write(data[i:(i+32)])	
			time.sleep(0.125)
			
		ser.flush()

	time.sleep(0.025)

###sock.close()
