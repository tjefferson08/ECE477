#import os, sys
#lib_path = os.path.abspath('../../lib/rpi/python-spi/build/lib.linux-armv6l-2.7')
#sys.path.append(lib_path)

#
#import spidev
import time


print "Hello"

spi = spidev.SpiDev()
spi.open(0,1)
#spi.max_speed_hz = 10000
try:
	while True:
		resp = spi.xfer2([0xAA])
#		resp = spi.xfer2([0xAA])
#		print resp[0]
		time.sleep(0.1)
except KeyboardInterrupt:
	spi.close()


