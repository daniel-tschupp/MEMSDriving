import serial

print("------------------ Send Start -----------------------")
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
print("Communication port:" + ser.name)

ser.write("{ \"RunScan\" }")

print(ser.read(500))

