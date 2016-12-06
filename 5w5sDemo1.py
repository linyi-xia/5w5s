import serial
import time
import sys
import datetime
import httplib, urllib

def get_file_name():  # new
	print "new file"
	return datetime.datetime.now().strftime("%Y-%m-%d_%H.%M.%S.txt")

filename = get_file_name()
f = open(filename,'w')
print f

ser = serial.Serial(
	#use the first one for Mac and 2nd for R-pi
   # port='/dev/tty.usbmodem1411',
	port='/dev/ttyACM0',
	baudrate=115200,
	parity=serial.PARITY_ODD,
	stopbits=serial.STOPBITS_TWO,
	bytesize=serial.SEVENBITS
)

out = ''
lstTime = datetime.datetime.now().minute

lstSecond = 0
while ser.isOpen() :
	time.sleep(1)
	second = datetime.datetime.now().second
	dataLine = ser.read()
	print dataLine
	currenttime = datetime.datetime.now().strftime("%H:%M:%S")
	f.write(currenttime)
	print currenttime, second, lstSecond
	f.write(dataLine)
	#lstSecond = second
	params = ''
	if (dataLine):
		second = datetime.datetime.now().second
		if dataLine=='M':
			print "sending M"
			motion = 1
			try:
				params = urllib.urlencode({'field1': motion, 'key':'UWVCMAFKPNB7SW0L'})
			except:
				print "urlencode failed"
		if dataLine=='m':
			motion = 0
			params = urllib.urlencode({'field1': motion, 'key':'UWVCMAFKPNB7SW0L'})
		if dataLine=='L':
			light = 1
			params = urllib.urlencode({'field2': light, 'key':'UWVCMAFKPNB7SW0L'})
		if dataLine=='l':
			light = 0
			params = urllib.urlencode({'field2': light, 'key':'UWVCMAFKPNB7SW0L'})

		# use your API key generated in the thingspeak channels for the value of 'key'
		# temp is the data you will be sending to the thingspeak channel for plotting the graph. You can add more than one channel and plot more graphs
		headers = {"Content-typZZe": "application/x-www-form-urlencoded","Accept": "text/plain"}
		conn = httplib.HTTPConnection("api.thingspeak.com:80") 

		#try to send data every 5 seconds if there's a change of outputs
		if (second-5)>lstSecond and len(params)>0:
			try:
				conn.request("POST", "/update", params, headers)
				response = conn.getresponse()
				print "Occupancy count sent "
				print response.status, response.reason
				data = response.read()
				conn.close()
				lstSecond=second
			except:
				print "Unexpected error:", sys.exc_info()[0]
				print "connection failed"
				raise


print "Serial is not available"

