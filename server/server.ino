#include <ESP8266WiFi.h>
#include <pins_arduino.h>
#include <DBH1.h>
#include <Servo.h>

#define SSID            "ESPGOBROOM"
#define PASSWORD        "lovestory"
#define CHANNEL         1
#define HIDDEN          false
#define MAX_CONNECTIONS 1
#define PORT            8000
#define TIMEOUT_MS      50

WiFiServer server(PORT);

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
  servo.write(NEUTRAL_ANGLE + angle)
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Init Motor-Controller and Servo
  dbh.init(D1, D2, D3, D4, D5, D6, A0, A0);
  servo.attach(D8)

  // Default IP is 192.168.4.1.
  WiFi.softAP(SSID, PASSWORD, CHANNEL, HIDDEN, MAX_CONNECTIONS);

  server.begin();
}

void loop() {
  // Turn LED *on* until a client connects.
  digitalWrite(LED_BUILTIN, LOW);

  WiFiClient client = server.available();
  if (!client) { return; }

  // Turn LED *off* after client connected.
  digitalWrite(LED_BUILTIN, HIGH);
  // Enable Motor
  dbh.EnableA();

  client.setTimeout(TIMEOUT_MS);
  while (client.connected()) {
    uint8_t[] msg[2];
    size_t num_bytes_rec = client.readBytes(msg, sizeof(msg));

    if (num_bytes_rec != sizeof(msg)) {
      // TIMEOUT OCCURED.
      drive(0);
      steer(0)
    } else {
      int8_t steeringAngle = msg[0];
      int8_t power = msg[1];
      steer(steeringAngle);
      if (power >= 0) { drive(floor(970*power/127.0f)); }
      else { reverse(floor(970*power/-128.0)); }
    }
  }
  dbh.DisableA();
}
