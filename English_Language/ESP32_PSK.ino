#include <WiFi.h>
const char* ssid     = "wifiname";
const char* password = "wifipassword";
String line;
const int led = 23;
const char* host = "arduino.php5.sk";
void setup()
{
    Serial.begin(115200);
    delay(10);
    pinMode(led, OUTPUT);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
void loop()
{
    delay(5000);

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/PHP_en/preklady.txt";


    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
         line = client.readStringUntil('\n');
        Serial.print(line);
    }
if (line=="On"){
 digitalWrite(led, HIGH);
  }else if (line=="Off"){
 digitalWrite(led, LOW);
  }
    Serial.println();
    Serial.println("closing connection");
}
