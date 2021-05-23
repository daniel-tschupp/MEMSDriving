import socket

TCP_IP = '192.168.1.1'
TCP_PORT = 5000
BUFFER_SIZE = 1024

print("Connecting to IP: " + str(TCP_IP) + " at port: " + str(TCP_PORT))
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))


msg = "{ \"MCUConfig\": { \"OCTType\": \"SpectrometerOCT\",\"AScanFrequency\": 20000,\"ScanningMode\": \"SingleScan\"}}"
print "sending:\n",  msg
s.send(msg)
data = s.recv(BUFFER_SIZE)
s.close()

print "received data:\n", data
