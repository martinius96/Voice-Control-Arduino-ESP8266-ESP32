#include "esp_wpa2.h"
#include <WiFi.h>
String line;
const char* ssid = "eduroam";
const char* host = "arduino.php5.sk";
#define EAP_IDENTITY "login@university.domain"
#define EAP_PASSWORD "password" 
const int led = 22;
void setup() {
    pinMode(led, OUTPUT);
    Serial.begin(115200);
    delay(10);
    Serial.println();
    Serial.print("Connecting to network ");
    Serial.println(ssid);
    WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide identity
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config to default (fixed for 2018 and Arduino 1.8.5+)
    esp_wifi_sta_wpa2_ent_enable(&config); //set config to enable function (fixed for 2018 and Arduino 1.8.5+)
   Serial.println("MAC address: ");
   Serial.println(WiFi.macAddress()); //Print our MAC address of our device
    WiFi.begin(ssid); //connect to Eduroam function
 WiFi.setHostname("ESP32Name"); //set Hostname for your device - not neccesary
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: "); 
    Serial.println(WiFi.localIP()); //print LAN IP

}
void loop() {
    delay(5000);
 if (WiFi.status() != WL_CONNECTED) { //if we lost connection, retry
        WiFi.begin(ssid);
        delay(500);        
    }
    Serial.print("Connecting to website: ");
    Serial.println(host);
    WiFiClient client;
    if (!client.connect(host, 80)) { // HTTP connection on port 80
        Serial.println("Connection lost! - Failed response");
    }
    String url = "/PHP_cs/preklady.txt"; //read .txt file    
    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println("Client timed out! - retry");
            client.stop();
        }
    }
    while(client.available()) {
         line = client.readStringUntil('\n');
        Serial.println(line);  
         if (line == "Zapni") { //zapnem vystup (rele alebo diodu)
    digitalWrite(led, HIGH);
  } else if (line == "Vypni") { //vypnem vystup (rele alebo diodu)
    digitalWrite(led, LOW);
  } else {
    Serial.println("Nepodporovana hlasova instrukcia, opakujte prikaz online");
  }
    }
    Serial.println();
    Serial.println("End connection");
}