import serial
print("------------------- Send IIR --------------------")
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
print("Communication port:" + ser.name)

fs = 10000
#gain = 0.1698
gain = 1/0.1698
a0 = 1.0
a1 = -0.39482
a2 = -0.60382
a3 = 0.40542
a4 = -0.066085
b0 = 2.2292
b1 = -4.9486
b2 = 2.7908
b3 = 0.64509
b4 = -0.65877


msg = "{ \"SetFilter\": { \"FilterName\": \"MagnMemsIIR10000\",\"Fs\": "+str(fs)+",\"Gain\": "+str(gain)+",\"a0\": "+str(a0)+",\"a1\": "+str(a1)+",\"a2\": "+str(a2)+",\"a3\": "+str(a3)+",\"a4\": "+str(a4)+",\"b0\": "+str(b0)+",\"b1\": "+str(b1)+",\"b2\": "+str(b2)+",\"b3\": "+str(b3)+",\"b4\": "+str(b4)+"}}"

print("Sending: " + msg)
ser.write(msg)
print(ser.read(500))

