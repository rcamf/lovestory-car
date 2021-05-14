#include <ESP8266WiFi.h>
#include <pins_arduino.h>

#define SSID            "ESPGOBROOM"
#define PASSWORD        "lovestory"
#define CHANNEL         1
#define HIDDEN          false
#define MAX_CONNECTIONS 1
#define PORT            8000
#define TIMEOUT_MS      10000

WiFiServer server(PORT);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

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

  client.setTimeout(TIMEOUT_MS);
  while (client.connected()) {
    uint8_t[] msg[2];
    size_t num_bytes_rec = client.readBytes(msg, sizeof(msg));

    if (num_bytes_rec != sizeof(msg)) {
      // TIMEOUT OCCURED.
      client.stop();
      // TODO: Stop engine.
    } else {
      // Echo the received message.
      client.write(msg, sizeof(msg));
    }
  }
}
