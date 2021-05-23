import serial
print("------------------- Start screening port --------------------")

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
print("Communication port:" + ser.name)

while 1:
	ser.write("Hallo Echo!\r\n");
	print(ser.read(500))

