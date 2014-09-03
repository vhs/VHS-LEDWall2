import SocketServer, serial
import time, os, random, datetime, sys, hexdump
import socket
import struct
import glob
import argparse

SocketServer.ThreadingTCPServer.allow_reuse_address = True

DEBUG = True 

LEDS_SIZE = 768
MAX_BYTES = (LEDS_SIZE*3)

HOST, PORT = '0.0.0.0', 9999

ser = serial.Serial('/dev/ttyACM0', 250000)
###logger = open( "data.log", 'w' );
print ser
###print logger

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
		###hexdump.hexdump( data )

		res = ser.write( data )
		print res

		while True:
			cts = ser.read()
			if DEBUG:
				print cts
			if cts == '#':
				print "<<#"
			if cts == '*':
				print "<<*"
				break;
		###if cts != '*' :
			###continue	
		###if DEBUG:
			###print "Writing " + str(len(data)) + " bytes of data"
		###res = ser.write( white_wall )
		ser.flush()

		###logger.write( data + "\n" )
		###if DEBUG:
			###print "Wrote " + str(res) + " bytes to serial"
			###print "In-wait:", ser.inWaiting()
		

	conn.close()

