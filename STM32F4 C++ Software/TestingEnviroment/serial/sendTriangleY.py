import serial

print("---------------- Send Triangle to CH2 -----------------------")
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
print("Communication port:" + ser.name)

msg = "{ \"TriangleSignal\": { \"OutputStage\": \"YAxis\",\"Frequency\": 135,\"Amplitude\": 0.5,\"Phase\": 0.0,\"Offset\": 0.0,\"Width\": 0.5,\"Filter\": \"MagnMemsIIR10000\"}}"

#msg = "{ \"TriangleSignal\": { \"OutputStage\": \"YAxis\",\"Frequency\": 135,\"Amplitude\": 0.5,\"Phase\": 0.0,\"Offset\": 0.0,\"Width\": 0.5,\"Filter\": \"NoFilter\"}}"

print("Sending: " + msg)
ser.write(msg)
print(ser.read(500))

