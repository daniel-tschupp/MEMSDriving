import serial
print("------------------- Send Config --------------------")
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
print("Communication port:" + ser.name)

msg = "{ \"MCUConfig\": { \"OCTType\": \"SpectrometerOCT\",\"AScanFrequency\": 20000,\"ScanningMode\": \"SingleScan\"}}"
print("Sending: " + msg)
ser.write(msg)
print(ser.read(200))

