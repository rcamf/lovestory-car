#include <ESP8266WiFi.h>
#include <pins_arduino.h>
#include <DBH1.h>
#include <Servo.h>
#include <WiFiUdp.h>

#define SSID            "ESPGOBROOM"
#define PASSWORD        "lovestory"
#define CHANNEL         1
#define HIDDEN          false
#define MAX_CONNECTIONS 1
#define PORT            8000

WiFiServer server(PORT);
WiFiUDP Udp;

uint8_t msg[2];

#define NEUTRAL_ANGLE 93

DBH1 dbh;
Servo servo;

void drive(int val) {
  dbh.ForwardA(val);
}

void reverse(int val) {
  dbh.ReverseA(val);
}

void steer(int angle) {
  servo.write(NEUTRAL_ANGLE + angle);
//  Serial.println(angle);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Init Motor-Controller and Servo
  dbh.init(D1, D2, D3, D4, D5, D6, A0, A0);
  servo.attach(D8);

  // Default IP is 192.168.4.1.
  WiFi.softAP(SSID, PASSWORD, CHANNEL, HIDDEN, MAX_CONNECTIONS);
  Serial.begin(115200);
  Udp.begin(PORT);

  dbh.EnableA();
}

void loop() {
  if (Udp.parsePacket()) {
    int n = Udp.read(msg, 2);
    int8_t steeringAngle = msg[0];
    int8_t power = msg[1];
    if (steeringAngle >= 0 ) { steer(floor(30*steeringAngle/127.0f)); }
    else { steer(floor(30*steeringAngle/128.0f)); }
    if (power >= 0) { drive(floor(970*power/127.0f)); }
    else { reverse(floor(970*power/-128.0f)); }
  } else {

  }
}
