import socket
import serial

UDP_IP = "192.168.188.10"
UDP_PORT = 12000
 
sock = socket.socket(socket.AF_INET, # Internet
                      socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

for i in range(10):
  j = i + 1
  try:
    ser = serial.Serial(('/dev/ttyACM' + str(j)), 9600)
    print('/dev/ttyACM' + str(j))
    break
  except:
    print "kein Arduino gefunden"

 
while True:
   data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
   print "received message:", data
   ser.write( data +'\n')
