import serial
print("---------------- Send Sine to CH1 -------------------")
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
print("Communication port:" + ser.name)

msg = "{ \"SineSignal\": { \"OutputStage\": \"XAxis\",\"Frequency\": 10.0,\"Amplitude\": 1.45,\"Phase\": 0.0,\"Offset\": 0.0,\"Filter\": \"NoFilter\"}}"

print("Sending: " + msg)
ser.write(msg)
print(ser.read(500))

