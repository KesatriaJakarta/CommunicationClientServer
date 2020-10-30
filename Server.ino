#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#define led1 16
#define MAX_CLIENTS 10
#define MAX_LINE_LENGTH 50
const char* ssid = "****";           // Nama AP/Hotspot
const char* password = "****";          // Password AP/Hotspot
bool toggle=0;    
int ledState = 0;
const char* oldLed = "";
const char* newLed = "";
const char* oldLed2 = "";
const char* newLed2 = "";
WiFiServer server(80);
WiFiClient *clients[MAX_CLIENTS] = { NULL };
char inputs[MAX_CLIENTS][MAX_LINE_LENGTH] = { 0 };
//ESP8266WebServer server(80);
//IPAddress ip (10, 10, 10, 1);
//IPAddress gateway (10, 10, 10, 1);
//IPAddress subnet (255, 255, 255, 0);
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(led1, OUTPUT);
  //pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  //digitalWrite(led2, LOW);
 // Settings WiFi ----------------------------------------------------------
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.mode(WIFI_AP);             // Mode AP/Hotspot
  WiFi.softAP(ssid, password);
 // Start the server -------------------------------------------------------
  server.begin();
  Serial.println("Server Started");
 // Print the IP address ---------------------------------------------------
  Serial.println(WiFi.localIP());
 }
void loop() {
  // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
    return;
 } 
  // Wait until the client sends some data
     Serial.println("new client");
      while(!client.available()){
      delay(1);
  }
  // Read the first line of the request -------------------------------------
   String req = client.readStringUntil('\r');
   Serial.println(req);
   //Command LED -------------------------------------------------------------
      if (req.indexOf("ledon") != -1){
         newLed = "ledon";
      }
      if (newLed != oldLed){
          digitalWrite(led1, HIGH);
          delay(500);
          digitalWrite(led1, LOW);
          oldLed = newLed;
       }
      else if (req.indexOf("ledoff") != -1){
      //else {
         newLed = "ledoff";      
      }    
   else {
   Serial.println("invalid request");
   client.stop();
   return;
   }
   client.flush();
   Serial.println("Client disonnected");
}
