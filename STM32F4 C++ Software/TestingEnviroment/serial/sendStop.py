import serial

print("---------------- Send Stop --------------------")
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
print("Communication port:" + ser.name)
ser.write("{ \"StopScan\"}")
print(ser.read(500))

