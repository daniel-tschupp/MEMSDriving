import socket

TCP_IP = '192.168.1.1'
TCP_PORT = 5000
BUFFER_SIZE = 1024

print("Connecting to IP: " + str(TCP_IP) + " at port: " + str(TCP_PORT))
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

# --------------- Config MCU ---------------------------------------------------------
msg = "{ \"MCUConfig\": { \"OCTType\": \"SpectrometerOCT\",\"AScanFrequency\": 20000,\"ScanningMode\": \"ContinuousScan\"}}"
print "sending:\n",  msg
s.send(msg)
data = s.recv(BUFFER_SIZE)

# -------------- Config X Signal -----------------------------------------------------

#msg = msg = "{ \"SineSignal\": { \"OutputStage\": \"XAxis\",\"Frequency\": 10.0,\"Amplitude\": 1.45,\"Phase\": 0.0,\"Offset\": 0.0,\"Filter\": \"NoFilter\"}}"
msg = "{ \"TriangleSignal\": { \"OutputStage\": \"XAxis\",\"Frequency\": 10,\"Amplitude\": 1.0,\"Phase\": 0.0,\"Offset\": 0.0,\"Width\": 0.5,\"Filter\": \"MagnMemsIIR10000\"}}"
#msg = "{ \"TriangleSignal\": { \"OutputStage\": \"XAxis\",\"Frequency\": 10,\"Amplitude\": 0.5,\"Phase\": 0.0,\"Offset\": 0.0,\"Width\": 0.5,\"Filter\": \"NoFilter\"}}"

print "sending:\n",  msg
s.send(msg)
data = s.recv(BUFFER_SIZE)

# ------------- Start cmd ------------------------------------------------------------

msg = "{ \"RunScan\" }"

print "sending:\n",  msg
s.send(msg)
data = s.recv(BUFFER_SIZE)

# ------------ Waiting for user input to stop again ----------------------------------

while(raw_input("Press x to stop signal: ") != "x"):
	continue

msg = "{ \"StopScan\"}"

print "sending:\n",  msg
s.send(msg)
data = s.recv(BUFFER_SIZE)

s.close()

print "received data:\n", data
