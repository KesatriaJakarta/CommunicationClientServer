#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define button 16
#define led1 5
//#define led 0
const char* ssid = "****";   // Nama SSID AP/Hotspot
const char* password = "****";       // Password Wifi
const char * host = "192.168.4.1";     // IP Server
int newbuttonstate = 0;
int oldbuttonstate = 0;
int state = 0;
//IPAddress * host = "192.168.4.1"; 
WiFiClient client;
//HTTPClient http;
//IPAddress ip (10, 10, 10, 21);
//IPAddress gateway (10, 10, 10, 1);
//IPAddress subnet (255, 255, 255, 0);
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(button, INPUT);
  //pinMode(led, OUTPUT);
  pinMode(led1, OUTPUT);

// Connect to WiFi network ------------------------------------------------
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
// Settings WiFi ----------------------------------------------------------
  WiFi.mode(WIFI_STA);                      // Mode Station
  WiFi.begin(ssid, password);               // Mencocokan SSID dan Password
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
// Print status Connect ---------------------------------------------------
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void loop(){
  newbuttonstate = digitalRead(button);
  const char* url;
  if ((newbuttonstate == HIGH)&&(oldbuttonstate == LOW)){
    state=1-state;
  }
    oldbuttonstate = newbuttonstate;

  if (state == HIGH) {
    url="ledoff";
    digitalWrite(led1, LOW);
  }
  else {
    url="ledon";
    digitalWrite(led1,HIGH); 
  }
int value = 0;
  delay(0);
  ++value;
  Serial.print("connecting to ");
  //Serial.println(host);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URL for the request  
  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url +" HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 10) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
}
