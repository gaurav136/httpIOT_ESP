#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <IRremote.h>

int pin;
int board;
int pinStatus;

int IR_RECV_PIN = D5;

#define IR_Button_1   0xFF30CF
#define IR_Button_2   0xFF18E7

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

bool toggleState_1 = LOW; //Define integer to remember the toggle state for relay 1
bool toggleState_2 = LOW; //Define integer to remember the toggle state for relay 2

void ir_remote(){
  if (irrecv.decode(&results)){
    switch (results.value){
      case (IR_Button_1):
        pin = 5;
        board = 1;
        pinStatus = toggleState_1;
        toggleState_1 = !toggleState_1;
        break;
        
      case (IR_Button_2):
        pin = 5;
        board = 2;
        pinStatus = toggleState_1;
        toggleState_1 = !toggleState_1;
        break;

      default : break;  
    }
    Serial.println("Hex code of button");
    Serial.println(results.value, HEX);
    Serial.println("pin :" +(String)pin +" board : " + (String)board + " pinStatus : " + (String)pinStatus);
    irrecv.resume();
  }
}

WiFiClient client;

long myChannelNumber = 1789116;
const char myWriteAPIKey[] = "MTZX12HOM1DLO4G8";



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  WiFi.begin("TIEC-SGGS", "Innovation@TIEC");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop()
{
  ir_remote(); //IR remote control 
  ThingSpeak.writeField(myChannelNumber, 1, pin, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, board, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3, pinStatus, myWriteAPIKey);
}
