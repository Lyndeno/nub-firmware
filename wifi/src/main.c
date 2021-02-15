#include <ESP8266WiFi.h>

const char *ap_ssid = "NUB";
const char *ap_password = "capstone1234";

// Initial setup code with assistance from https://tttapa.github.io/ESP8266/Chap07%20-%20Wi-Fi%20Connections.html
void setup() {
    Serial.begin(115200); // this value is to be decided
    //delay(5);
    Serial.println('\n');

    WiFi.softAP(ap_ssid, ap_password);
    
    // TODO: get this codeblock into one println statement
    Serial.print("Access Point \"");
    Serial.print(ap_ssid);
    Serial.println("\" started");

    // TODO: same with this
    Serial.print("IP Address:\t");
    Serial.println(WiFi.softAPIP());
}

void loop () {
    
}

void mcu_receive() {

}

void mcu_send() {

}

void phone_receive() {

}

void phone_send() {

}
