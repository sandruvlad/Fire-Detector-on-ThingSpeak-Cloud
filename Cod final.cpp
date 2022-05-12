#include <SPI.h>
#include <WiFi101.h>                              // Wifi libreary for mkr1000
WiFiClient client;                                 // Wifi client 

String apiKey = "JJ705Z70KSMRDFKZ";                // Write API
const char *ssid = "VM1635798";                   // wifi and password
const char *pass = "Jn6vmszkbvvp";
const char* server = "api.thingspeak.com";       //thingspeak server

#include "DHT.h"                                   // DHT sensor
#define DHTPIN 1                                    // dht to pin 1
#define DHTTYPE DHT11  // DHT 11                     // dht type dht11
DHT dht(DHTPIN,DHTTYPE);

void setup()
{
    Serial.begin(9600);
    delay(1000);
    Serial.print("Connecting to ");                       // Serial monitor print 
    Serial.print(ssid);   
    dht.begin();                                          // dht sensor starting
    WiFi.begin(ssid, pass);                               // accessing the wifi
    while (WiFi.status() != WL_CONNECTED)                 // check status
    delay(500);
    Serial.print("");
    Serial.println("WiFi connected");                      // serial print that wifi connected
}
void loop() {
    delay(500);
    int h = dht.readHumidity();                           // adding all the sensors 
    int t = dht.readTemperature();                        // dht
    int s = analogRead (A2);                              // smoke
    int c = analogRead(A1);                               //co2 mq7
 
if (client.connect(server, 80))                  // starting connection witht he server through port 80
{
    String postStr = apiKey;                  // assigining all the variables with the fields created on the channel
    postStr += "&field1=";
    postStr += String (c*10);
    postStr += "&field2=";
    postStr += String (t);
    postStr += "&field3=";
    postStr += String (h);
    postStr += "&field4=";
    postStr += String (s);
    postStr += "r\n";

    Serial.println("Data Sent to Thingspeak");          // communicating with the server (docummentation setup)
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    client.stop();
    delay(15000);                                       // delay of 15 sec
  }
}