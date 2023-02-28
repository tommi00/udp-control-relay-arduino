# udp-control-relay-arduino
Code to control via network (via UDP protocol) a relay connected to an Arduino Uno, which mounts an ethernet shield. 

Created by tommi00 @ GitHub -> https://tommi00.github.io/t 

Inspiration taken from: https://docs.arduino.cc/tutorials/ethernet-shield-rev2/udp-send-receive-string 

>> TCP VERSION: https://github.com/tommi00/tcp-control-relay-arduino

**I tested the code with an Arduino Uno, an Ethernet Shield & a relay module, connected to my network on ip 192.168.2.50 and port 9001. 
  To test the message received and the ack sent by Arduino, I use the software "PACKET SENDER".
  It works well, except when I tried to send many equal commands, one after the other quickly. The Arduino had some problem to read the next different message, indeed it added another char if the length was lower. So I had to change the on command to "onn". 
  For example, if I try to send a lot of "off" strings quickly and after them immediately an "on" (or more than one withouth waiting the buffer to empty), the Arduino reads a command "onf", beacuse it tries to receive another three char command and adds as third char the last third char received, which is the last "f" from the previous "off" commands sent. Changing the expected message to "onn" which has the same length, seem to solve the problem.

- V1: first version + changed to "onnnnnn" & "offffff" to avoid problems with HEX possible messages (e.g. #ffffff).
- V2: added different ack when turned on and off -> ackOn, ackOff 
