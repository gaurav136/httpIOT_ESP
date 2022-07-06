#include<ESP8266WiFi.h>

WiFiClient client;
WiFiServer server(80);

#define led1 LED_BUILTIN
#define led2 D6

void setup()
{
  Serial.begin(9600);
  WiFi.begin("TIEC-SGGS", "Innovation@TIEC");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  server.begin();
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}
void loop()
{
  client = server.available(); //Gets a client that is connected to server and
                              //has data available for reading
  if(client ==1){
    String request = client.readStringUntil('\n');
    Serial.println(request);
    request.trim();
    if(request == "GET /led1on HTTP/1.1")
    {
      digitalWrite(led1, LOW); 
    }
    if(request == "GET /led1off HTTP/1.1")
    {
      digitalWrite(led1, HIGH); 
    }
    if(request == "GET /led2on HTTP/1.1")
    {
      digitalWrite(led2, HIGH);
    }
    if(request == "GET /led2off HTTP/1.1")
    {
      digitalWrite(led2, LOW);
    }
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1>Local Network Iot</h1>");
  client.println("<h3> Remote Controls </h3>");
  client.println("<br>");
  client.println("<a href=\"/led1on\"\"><button> LED ON </button></a>");
  client.println("<a href=\"/led1off\"\"><button> LED OFF </button></a><br/>");
  client.println("<a href=\"/led2on\"\"><button>BUZZER ON</button></a>");
  client.println("<a href=\"/led2off\"\"><button>BUZZER OFF</button></a><br/>");
  client.println("</html>");  
}
