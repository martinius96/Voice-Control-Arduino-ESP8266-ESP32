//CORE 2.3.0+, 2.5.0 compatible
//WORKING ON PHP5.sk!!!!
//Author: Martin Chlebovec
//Website: https://arduino.php5.sk
#include <ESP8266WiFi.h>
const int led = 16; //GPIO 16 = D0 on NodeMCU board
const char * ssid = "WIFI_SSID";
const char * password = "WIFI_PASSWORD";
const char * host = "www.arduino.php5.sk"; //bez https a www
const int httpPort = 80; //https port
void setup() {
  Serial.begin(9600);
  Serial.println();
  pinMode(led, OUTPUT);
  Serial.print("pripajam na wifi siet: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi uspesne pripojene");
  Serial.println("IP adresa: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  WiFiClient client; //
  Serial.print("pripajam sa na server ");
  Serial.println(host);
  client.stop();
  if (!client.connect(host, httpPort)) {
    Serial.println("pripojenie neuspesne");
    return;
  }
  String url = "/PHP_en/preklady.txt";
  Serial.print("Request to address: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: NodeMCU\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("Request taken");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    // NO NEED TO PRINT HTTP HEADER... SKIP
    if (line == "\r") {
      Serial.println("Response comming");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  Serial.println("Response: ");
  Serial.println(line);
  if (line == "On") { //turn on relay/led
    digitalWrite(led, HIGH);
  } else if (line == "Off") { //turn off relay/led
    digitalWrite(led, LOW);
  } else {
    Serial.println("Not supported voice command");
  }
  delay(5000);
}
