#include <ESP8266WiFi.h>

void setup(){
  Serial.begin(9600);
  WiFi.begin("Galaxy M219979", "euzf9822");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.println("NodeMcu is Connected");
  Serial.println(WiFi.localIP());
}

void loop(){
  
}
