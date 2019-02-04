#include <ESP8266WiFi.h> //kniznica importovana v Arduine core, testovana verzia 2.3.0
#include <WiFiClientSecure.h> //kniznica importovana v Arduine core, testovana verzia 2.3.0
const int led = 16; //GPIO 16 = D0 on NodeMCU board
const char * ssid = "menowifi";
const char * password = "heslowifi";
const char * host = "arduino.php5.sk"; //bez https a www
const int httpsPort = 443; //https port
const char * fingerprint = "13 9f 87 1d b1 85 be e6 bd 73 c1 8d 04 63 58 99 f0 32 43 92"; // odtlacok certifikatu SHA1
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
  WiFiClientSecure client; //funkcia pre HTTPS spojenia
  Serial.print("pripajam sa na server ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("pripojenie neuspesne");
    return;
  }
  if (client.verify(fingerprint, host)) {
    Serial.println("certifikat zhodny");
  } else {
    Serial.println("certifikat nezhodny");
  }
  String url = "/PHP_cz/preklady.txt";
  Serial.print("poziadavka na adresu: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "User-Agent: NodeMCU\r\n" +
    "Connection: close\r\n\r\n");
  Serial.println("poziadavka vykonana");
  while (client.connected()) {
    String line = client.readStringUntil('\n');

    if (line == "\r") {
      Serial.println("Response prijata");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  Serial.println("Vratena premenna: ");
  Serial.println(line);
  if (line == "Zapni") { //zapnem vystup (rele alebo diodu)
    digitalWrite(led, HIGH);
  } else if (line == "Vypni") { //vypnem vystup (rele alebo diodu)
    digitalWrite(led, LOW);
  } else {
    Serial.println("Nepodporovana hlasova instrukcia, opakujte prikaz online");
  }
}
