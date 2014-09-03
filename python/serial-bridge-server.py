##
## Created by Ty Eggen <tycho.eggen@gmail.com>
##
import SocketServer, serial
import time, os, random, datetime, sys, hexdump
import socket
import struct
import glob
import argparse

SocketServer.ThreadingTCPServer.allow_reuse_address = True

DEBUG = False 

LEDS_SIZE = 768
MAX_BYTES = (LEDS_SIZE*3)

HOST, PORT = '0.0.0.0', 9999

ser = serial.Serial('/dev/ttyACM0', 512000)
###ser = open( "test.txt", 'rw' );
print ser

try:
	sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
except socket.error as msg:
 	sock = None

# Create the server, binding to localhost on port 9999
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
try:
	sock.bind((HOST, PORT))
	try:
		sock.listen(5)
	except socket.error as msg:
		sock.close
		sock = None
except socket.error as msg:
	sock.close()
	sock = None

emptyframe = []
	
if not sock:
	print "ERROR: Invalid socket"
	sys.exit()

white_wall = "" + (chr(255) * MAX_BYTES);

while sock:
	try:
		conn, addr = sock.accept()
	except socket.error as msg:
		sock.close()
		sock = None
		break

	while True:
		data = conn.recv( MAX_BYTES )
		if not data: break

		while True:
			cts = ser.read()
			if DEBUG:
				print cts,
			if cts == '*':
				break

		res = ser.write( data )
	if len(data) < 2304:
		for i in range(0,(2304-len(data))):
			res=ser.write(chr(0))
	conn.close()

