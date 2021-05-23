import serial
print("----------- Send Get State ------------------------")
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
print("Communication port:" + ser.name)
ser.write("{ \"GetState\" }")
print(ser.read(500))

