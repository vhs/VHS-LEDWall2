##
## Created by Ty Eggen <tycho.eggen@gmail.com>
##
import socket
import sys
import time
import random
import base64

""" Gradient pattern """

HOST, PORT = "localhost", 9999
FIXTURES = 768
CHANNELS = FIXTURES*3

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

gradient_canvas = []
for i in range(0,CHANNELS):
        gradient_canvas.append(0)

starburst_canvas = []
for i in range(0,CHANNELS):
        starburst_canvas.append(0)

canvas = []
for i in range(0,CHANNELS):
        canvas.append(0)

fp = open( 'running', 'w' )
fp.write( '' )
fp.close()

black = ''
for i in range(0,FIXTURES):
        black = black + chr(0)
        black = black + chr(0)
        black = black + chr(0)

running = True
counter = 0

low_r = 0
low_g = 0
low_b = 0

max_r = 255
max_g = 255
max_b = 255

r = random.randint(low_r,max_r)
g = random.randint(low_g,max_g)
b = random.randint(low_b,max_b)

dest_r = random.randint(low_r,max_r)
dest_g = random.randint(low_g,max_g)
dest_b = random.randint(low_b,max_b)

if dest_r > r:
        r_delta = 1
else:
        r_delta = -1

if dest_g > g:
        g_delta = 1
else:
        g_delta = -1

if dest_b > b:
        b_delta = 1
else:
        b_delta = -1

while running:

	# Gradient

        if (r+r_delta) == dest_r or r == low_r or r >= max_r:
                dest_r = random.randint(low_r,max_r)
                if dest_r >= r:
                        r_delta = 1
                else:
                        r_delta = -1
        r = r+r_delta

        if (g+g_delta) == dest_g or g == low_g or g >= max_g:
                dest_g = random.randint(low_g,max_g)
                if dest_g > g:
                        g_delta = 1
                else:
                        g_delta = -1
        g = g+g_delta

        if (b+b_delta) == dest_b or b == low_b or b >= max_b:
                dest_b = random.randint(low_b,max_b)
                if dest_b > b:
                        b_delta = 1
                else:
                        b_delta = -1
        b = b+b_delta

        # Save data
        for c in range( 0, FIXTURES ):
		r_channel = c*3
		g_channel = r_channel+1
		b_channel = g_channel+1
		###print r_channel, r
		###print g_channel, g
		###print b_channel, b
                gradient_canvas[r_channel] = r ## Red
                gradient_canvas[g_channel] = g ## Green
                gradient_canvas[b_channel] = b ## Blue

	# Starburst

        for c in range(0,CHANNELS):
                if starburst_canvas[c] > 128:
                        starburst_canvas[c] = starburst_canvas[c] - 8
                elif starburst_canvas[c] > 64:
                        starburst_canvas[c] = starburst_canvas[c] - 4
                elif starburst_canvas[c] > 0:
                        starburst_canvas[c] = starburst_canvas[c] - 1

        if counter == random.randint( 0, 8 ):
                for i in range(0,random.randint(1,5)):
                        random_pixel = random.randint( 1, (FIXTURES-1) )

                        r_pixel = (random_pixel*3)
                        g_pixel = r_pixel + 1
                        b_pixel = g_pixel + 1
                        starburst_canvas[r_pixel] = random.randint(0,255)
                        starburst_canvas[g_pixel] = random.randint(0,255)
                        starburst_canvas[b_pixel] = random.randint(0,255)

        for c in range(0,CHANNELS):
		canvas[c] = gradient_canvas[c]
		if starburst_canvas[c] > 0:
			canvas[c] = starburst_canvas[c]

        data = ''
        for c in range(0,CHANNELS):
		###print c, canvas[c]
                data = data + chr(int(canvas[c]))

        # Send data
        try:
                sock.send(data)
        except socket.error as msg:
                print msg
                break

        # Mandatory sleep
        time.sleep(0.0225)

        ## Check if we can still run
        fp = open( 'running', 'r' )
        inp = fp.read().strip()
        if inp == "STOP":
                running = False
        fp.close()

# Close socket
sock.close()
