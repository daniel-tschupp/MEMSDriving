import serial, time

def rw(msg, timeout):
	print("Sending: " + msg)
	ser.write(msg)
	ans = ser.read(timeout)
	print(ans)
	return ans

print("------------------- Communicate through Esp8266 Wifi Module using AT Commands: --------------------")
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
print("Communication port:" + ser.name)

rw("AT\r\n", 100)
rw("AT+CWJAP?\r\n", 100)


