#include <ESP8266WiFi.h>

const char *ssid = "NUB";
const char *password = "capstone1234";

// Initial setup code with assistance from 
void setup() {
    Serial.begin(115200); // this value is to be decided
    delay(10);
    Serial.println('\n');

    WiFi.softAP(ssid, password);
    Serial.print("Access Point \"");
    Serial.print(ssid);
    Serial.println("\" started");

    Serial.print("IP Address:\t");
    Serial.println(WiFi.softAPIP());
}

void loop () {
    
}