import os, sys
lib_path = os.path.abspath('../../lib/rpi/python-spi/build/lib.linux-armv6l-2.7')
sys.path.append(lib_path)

import spidev
import time


print "Hello"

spi = spidev.SpiDev()
spi.open(0,0)
while True:
	resp = spi.xfer2([0xAA])
	print resp[0]
	time.sleep(1)

