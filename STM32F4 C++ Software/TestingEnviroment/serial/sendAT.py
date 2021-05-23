import serial
print("------------------- Send AT Command: AT --------------------")
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
print("Communication port:" + ser.name)

msg = "AT\r\n\0"
print("Sending: " + msg)
ser.write(msg)
print(ser.read(3000))

