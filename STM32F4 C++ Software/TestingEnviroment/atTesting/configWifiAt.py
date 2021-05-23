import serial, time

def rw(msg, timeout):
	print("Sending: " + msg)
	ser.write(msg)
	ans = ser.read(timeout)
	print(ans)
	return ans

print("------------------- Config Esp8266 Wifi Module using AT Commands: --------------------")
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
print("Communication port:" + ser.name)

rw("AT\r\n", 100)
rw("AT+RST\r\n", 1000)
rw("ATE0\r\n", 100)
rw("AT+GMR\r\n", 100)
rw("AT+CWMODE=1\r\n", 100)
#rw("AT+CWLAP\r\n", 5000)
rw("AT+CWJAP=\"Skynet\",\"Morgul92\"\r\n", 100)
rw("AT+RST\r\n", 1000)

time.sleep(3)
rw("AT+CWJAP?\r\n", 100)
rw("AT+CIFSR\r\n", 500)

rw("AT+CIPMUX=1\r\n",100)
rw("AT+CIPSERVER=1\r\n",500)

