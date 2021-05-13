#include <DBH1.h>
#include <Servo.h>
#include "ESP8266WiFi.h"

#define PWM_MAX_DUTY 1020

const char* ssid = "ssid";
const char* password = "password";

WiFiServer wifiServer(80);
DBH1 dbh;
Servo servo;

void drive(int val) {
    dbh.ForwardA(min(val, PWM_MAX_DUTY));
}

void setup() {
    Serial.begin(115200);
    dbh.init(D1, D2, D3, D4, D5, D6, A0, A0);
    servo.attach(D8);
    delay(500)
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    
    wifiServer.begin();
}

void loop() {
    WiFiClient client = wifiServer.available();
    if (client) {
        while(client.connected()) {
            while(client.available()>0) {
                String line = client.readStringUntil('\n');
                Serial.write();
            }
            delay(10);
        }
        client.stop();
    }
}