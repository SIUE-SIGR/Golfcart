import serial
import time
from time import sleep # "sleep(seconds)"
from struct import *

port = serial.Serial("/dev/ttyUSB0", 19200)
serial.EIGHTBITS
serial.PARITY_NONE
serial.STOPBITS_ONE

port.write(b'\xaa') #170 for baud rate character 
address = 1 #initialize not 0

while 1:
	address = int(input("Enter Address: "))
	if address != 0:
		command = int(input("Enter Command: "))
		data = int(input("Enter Speed: "))
		checksum = ((address + command + data) & 127)

		hexa = format(address,'02x')
		hexc = format(command,'02x')
		hexd = format(data,'02x')
		hexs = format(checksum,'02x')

		print address
		print command
		print data
		print checksum
		print ''
		print "Address in hex:  ",hexa
		print "Command in hex:  ",hexc
		print "Data in hex:     ",hexd
		print "Checksum in hex: ",hexs

		barray = [address,command,data,checksum]
		x = bytearray(barray)
		port.write(x)
		#packet = pack('c','\x80\x00\x28\x28')
		#print packet
		#port.write(packet)
	else: port.write(b'\x80\x00\x00\x00')# 130, 0, 64, 02(checksum) STOP


port.write(b'\x80\x00\x00\x00')# 130, 0, 64, 02(checksum) STOP
sleep(1)
#port.write(b'\x82\x00\x00\x02')# 130, 0, 64, 02(checksum) STOP

port.close()
exit()
	

