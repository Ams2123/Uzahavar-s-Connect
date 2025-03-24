#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Ams";     
const char* password = "n4awc3fs";

ESP8266WebServer server(80);

void setup(){
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {delay(1000);Serial.print('.');}
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  server.on("/home",HTTP_GET,handleControl);
}

void loop(){
  server.handleClient();
}

void handleControl(){
  String direction = server.arg("direction");
  Serial.println(direction);
  server.sendHeader("Access-Control-Allow-Origin", "*");  // Allow all domains (adjust as needed)
  server.sendHeader("Access-Control-Allow-Methods", "GET"); 
  if(Serial.available())
    server.send(200,"text/plain",Serial.readString());
  else
    server.send(200,"text/plain","no-data");
}

