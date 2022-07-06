#include<ESP8266WiFi.h>

WiFiClient client;
WiFiServer server(80);

#define led D5

void setup(){
  Serial.begin(9600);
  WiFi.softAP("NodeMCU", "123456789");
  Serial.println();
  Serial.println("NodeMCU Started!");
  Serial.println(WiFi.softAPIP());
  server.begin();
  pinMode(led, OUTPUT);
}

void loop()
{
  client = server.available(); //Get client that connected to server and
                               //has data available for reading 
  if(client == 1)
  {
    String request = client.readStringUntil('\n');
    Serial.println(request);
    request.trim();
    if(request == "GET /ledon HTTP/1.1"){
      digitalWrite(led, HIGH);
    }
    if(request == "GET /ledoff HTTP/1.1")
    {
      digitalWrite(led, LOW);
    } 
  }
}
