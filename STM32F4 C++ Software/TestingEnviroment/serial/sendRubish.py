import serial
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
print("Communication port:" + ser.name)
ser.write("asfgasdfgahsglepro")
print(ser.read(2000))

