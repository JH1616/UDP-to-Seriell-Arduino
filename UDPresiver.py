import socket
import serial #benoetigt pyseriell (hierzu sudo python setup.py install)

UDP_IP = "192.168.188.10" #eigene IP
UDP_PORT = 12000 #eigener Port
 
sock = socket.socket(socket.AF_INET, # Internet
                      socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

#zum automatischen finden des arduinos unter linux
for i in range(10): 
  j = i + 1 #/dev/ttyACM0 bei uns durch irgendein anderes geraet schon vergeben
  try:
    ser = serial.Serial(('/dev/ttyACM' + str(j)), 9600) #9600 baud
    print('/dev/ttyACM' + str(j))
    break
  except:
    print (str('/dev/ttyACM' + str(j) + "kein Arduino gefunden"))

 
while True:
   data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
   print "received message:", data
   ser.write( data +'\n')
