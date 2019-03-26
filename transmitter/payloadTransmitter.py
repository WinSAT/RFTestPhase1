import time
#import serial
#ser = serial.Serial("/dev/ttyACM0",9600)

while True:
	usrInput = input("Enter txt to send: ")
	print usrInput
	#ser.write(str(usrInput))
	time.sleep(0.5)


''''
f = open("base.txt","r")
imgData = f.read()
bufSize = 1
imgDataStream = [ imgData[i:i+bufSize] for i in range(0,len(imgData),bufSize)]

#with open("imageToSave.png", "wb") as fh:
#    fh.write(img_data.decode('base64'))

for i in imgDataStream:
    print "Sending: ", i
    ser.write(str(imgDataStream))
    time.sleep(0.5)
'''

for i in range(1000):
    print "Sending: ", i
    ser.write(str(i))
    time.sleep(0.5)