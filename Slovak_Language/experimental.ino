/*|---------------------------------|*/
/*|Projekt: Ovládanie hlasom        |*/
/*|Autor: Martin Chlebovec          |*/
/*|E-mail: martinius96@gmail.com    |*/
/*|Web: https://arduino.php5.sk     |*/
/*|Licencia pouzitia: MIT           |*/
/*|---------------------------------|*/
#include <SPI.h>
#include <Ethernet.h>
int led = 13;
byte mac[] = { 0xAA, 0xBB, 0xCC, 0x81, 0x7B, 0x4A }; //fyzicka adresa MAC
char serverName[] = "www.arduino.php5.sk"; // webserver
IPAddress ip(192, 168, 0, 88);
EthernetClient client;
void setup(){
pinMode(led, OUTPUT);
 if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP nepridelilo adresu, skusam so statickou...");
    Ethernet.begin(mac, ip);
  }
  Serial.begin(115200); 
}

void loop(){
if (client.connect(serverName, 80)) {  //starts client connection, checks for connection
    Serial.println("Pripojene");
    client.println("GET /PHP_sk/preklady.txt HTTP/1.1"); //download text
    client.println("Host: www.arduino.php5.sk");
    client.println("Connection: close");  // ukonc HTTP/1.1 spojenie
    client.println(); //koneic requestu
    while (client.connected()) {
      String hlavicka = client.readStringUntil('\n');
      Serial.println(hlavicka);
      if (hlavicka == "\r") {
        break;
      }
    }
   String premenna = client.readStringUntil('\n');
   Serial.println("Premenna je:");
   Serial.println(premenna); 
   if(premenna=="Vypni"){
    Serial.println("Vypinaj");
    }else if(premenna=="Zapni"){
      Serial.println("Zapinaj");
      }
  } 
  else {
    Serial.println("Pripojenie neuspesne"); //chyba ak nie som pripojeny
    Serial.println();
  }
  client.stop(); //ukonc spojenie
  delay(5000); //pockaj 5s a vykonaj novu slucku loop
} 
