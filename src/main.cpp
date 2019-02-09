
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "Shreeji";
const char *password = "9998328464Mm";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[10];  // buffer for incoming packets
char replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back
int t = 100;//turn delay
int d = 150;// direction delay
char state_old_1 = 'n';
char state_old_2 = 'n';

void getData();

void initialization() {
    pinMode(D1, OUTPUT);
    digitalWrite(D1, LOW);
    pinMode(D2, OUTPUT);
    digitalWrite(D2, LOW);
    pinMode(D3, OUTPUT);
    digitalWrite(D3, LOW);
    pinMode(D4, OUTPUT);
    digitalWrite(D4, LOW);
}

void left() {
    digitalWrite(D1, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    Serial.println("FORWARD");
    // server.send(200, "text/plain", "FORWARD");
}

void stop() {
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    Serial.println("STOP");
    // server.send(200, "text/plain", "STOP");
}

void right() {

    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D1, LOW);
    digitalWrite(D4, LOW);
    Serial.println("BACK");
    // server.send(200, "text/plain", "BACK");
}

void forward() {
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
    Serial.println("RIGHT");
//  server.send(200, "text/plain", "RIGHT");
}

void back() {
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    Serial.println("LEFT");
    // server.send(200, "text/plain", "LEFT");
}

void WifiConnect() {
    Serial.printf("Connecting to %s ", ssid);
    WiFi.disconnect(true);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected");

}

void setup() {
    Serial.begin(115200);
    Serial.println();
    initialization();
    WifiConnect();
    Udp.begin(localUdpPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop() {
    getData();
}

void getData() {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
        // receive incoming UDP packets
        Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(),
                      Udp.remotePort());
        int len = Udp.read(incomingPacket, 255);
        /* if (len > 0) {
             incomingPacket[len] = 0;
         }*/

        Serial.printf("UDP packet contents: %s\n", incomingPacket);
        switch (incomingPacket[0]) {
            case 's':
                stop();
                break;
            case 'f':
                forward();
                break;
            case 'l':
                left();
                break;
            case 'r':
                right();
                break;
            case 'b':
                back();
                break;
            case 'S':
                stop();
                break;
            case 'F':
                forward();
                delay(d);
                stop();
                break;
            case 'L':
                left();
                delay(t);
                stop();
                break;
            case 'R':
                right();
                delay(t);
                stop();
                break;
            case 'B':
                back();
                delay(d);
                stop();
                break;
            default:
                break;
        }
        incomingPacket[0] = ' ';
         char tmp[10];
         tmp[0]=incomingPacket[4];
         tmp[1]=incomingPacket[5];
         tmp[2]=incomingPacket[6];
         incomingPacket[4]=' ';

         incomingPacket[5]=' ';
         incomingPacket[6]=' ';

         String intt(tmp);
         String ints(incomingPacket);
         int d = ints.toInt();
         int t=intt.toInt();
        Serial.printf("d : %d   t : %d\n ", d, t);

        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.print(replyPacket);
        Udp.endPacket();
    }
}