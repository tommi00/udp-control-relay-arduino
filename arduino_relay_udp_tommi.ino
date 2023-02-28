#include <Ethernet.h>
#include <EthernetUdp.h>

const int RELAY_PIN  = 9;                             // relay pin
const char relayOn[] = "onnnnnn";                     //READ THE READ ME FILE TO UNDESTAND 
const char relayOff[] = "offffff";

int relayState = HIGH;                                 // initial status of the relay

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // mac address (keep this one when unknown)
byte ip[] = { 192, 168, 2, 50 };                      // ip address
byte gateway[] = { 192, 168, 2, 1 };                  // gateway rete
byte subnet[] = { 255, 255, 255, 0 };                 // subnet mask

unsigned int localPort = 9001;                        // local port to listen on

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];            // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";                  // a string to send back
char ReplyOn[] = "ackOn";                              // ack only for when is turned on
char ReplyOff[] = "ackOff" ;                           // ack only for when is turned off


// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  Ethernet.begin(mac, ip);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  //INITIAL CHECKS:
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start UDP
  Udp.begin(localPort);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i=0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    //Serial.println("expected command:");
    //Serial.println(relayOn);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    //Serial.println("comp:");
    //Serial.println(strcmp(relayOn, packetBuffer));

    if(!strcmp(relayOn, packetBuffer) ){
      Serial.println("command ON received");
      relayState = LOW;
      digitalWrite(RELAY_PIN, relayState);
      Serial.println("relay turned on");

      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(ReplyOn);
      Udp.endPacket();

    }else if (!strcmp(relayOff, packetBuffer) ){
      Serial.println("command OFF received");
      relayState = HIGH;
      digitalWrite(RELAY_PIN, relayState);
      Serial.println("relay turned off");

      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(ReplyOff);
      Udp.endPacket();
    }

    /*
    // send a reply to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
    */
  }
  delay(10);
}
