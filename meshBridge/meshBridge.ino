#include<ESP8266WiFi.h>
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

// Variables
int led;
bool led_status;
int board_status;
int board;
int pin;
bool pin_status;
String message = "";

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 10 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  DynamicJsonDocument doc(1024);
  doc["board"] = board_status;
  doc["pin"] = led;
  doc["pin_status"] =  led_status;
  String msg ;
  serializeJson(doc, msg);
  mesh.sendBroadcast( msg );
  Serial.print("Mesh Broadcast - "); 
  Serial.println(msg);

 // taskSendMessage.setInterval((TASK_SECOND * 1));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}


WiFiClient client;
WiFiServer server(80);

#define led1 LED_BUILTIN
#define led2 D6

void setup()
{
  Serial.begin(115200);
  WiFi.begin("G", "12345678");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  server.begin();

  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  
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
      board_status = 1;
      led = 5;
      led_status = LOW; 
    }
    if(request == "GET /led1off HTTP/1.1")
    {
      board_status = 1;
      led = 5;
      led_status = HIGH; 
    }
    if(request == "GET /led2on HTTP/1.1")
    {
      board_status = 2;
      led = 5;
      led_status = HIGH;
    }
    if(request == "GET /led2off HTTP/1.1")
    {
      board_status = 2;
      led = 5;
      led_status = LOW;
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

  mesh.update();
}
