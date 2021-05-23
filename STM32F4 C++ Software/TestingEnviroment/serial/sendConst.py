import serial
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
print("Communication port:" + ser.name)

msg = "{ \"ConstantSignal\": { \"OutputStage\": \"XAxis\",\"Duration\": 0.1,\"DCValue\": 1.2,\"Offset\": 0.0,\"Filter\": \"NoFilter\"}}"

print("Sending: " + msg)
ser.write(msg)
print(ser.read(2000))

