#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
#include<IRremote.h>
int pin;
int board;
int pinStatus;
String Data;

int IR_RECV_PIN = D5;
int priPin = 0;
int priBoard = 0;
int priPinStatus =0;
int httpCode1 = 0;
int httpCode2 = 0;
int httpCode3 = 0;

#define IR_Button_1 0xFF30CF
#define IR_Button_2 0XFF18E7

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

int toggleState_1 = 0;
int toggleState_2 = 0;

void ir_remote() {
  if (irrecv.decode(&results)) {
    switch (results.value) {
      case (IR_Button_1):
        pin = 5 ;
        board = 1;
        pinStatus = toggleState_1;
        if(toggleState_1 == 0){
        toggleState_1 = 1;
        }
        else{
          toggleState_1 = 0;
        }
        Data = "pin: " + (String)pin + " board : " + (String)board + " pinStatus : " + (String)pinStatus;
        Serial.println(Data);
        break;

      case (IR_Button_2):
        pin = 6;
        board = 2;
        pinStatus = toggleState_2;
        if(toggleState_2 == 0){
        toggleState_2 = 1;
        }
        else{
          toggleState_2 = 0;
        }
        Data = "pin: " + (String)pin + " board : " + (String)board + " pinStatus : " + (String)pinStatus;
        Serial.println(Data);
        break;
      default : break;
    }

    Serial.println("Hex code of button");
    Serial.println(results.value, HEX);
    irrecv.resume();

  }
}

String host = "api.thingspeak.com";
int httpPort = 80;
String url1 = "/update?api_key=MTZX12HOM1DLO4G8&field1=";
String url2 = "/update?api_key=MTZX12HOM1DLO4G8&field2=";
String url3 = "/update?api_key=MTZX12HOM1DLO4G8&field3=";

HTTPClient http;
WiFiClient client;
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  WiFi.begin("G", "12345678");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.println("Node Mcu is Connected");
  Serial.println(WiFi.localIP());

}
void loop() {
  ir_remote();
  
  if (priPin != pin) {
    while (httpCode1 != 200) {
      url1 = url1 + (String)pin;
      http.begin(client, host, httpPort, url1);
      httpCode1 = http.GET();
      Serial.print("httpCode1 : ");
      Serial.println(httpCode1);
      priPin = pin;
      url1 = "/update?api_key=MTZX12HOM1DLO4G8&field1=";
    }
    httpCode1 = 0;
  }
  /*
  if (priBoard != board) {
    while (httpCode2 != 200) {
      url2 = url2 + (String)board;
      http.begin(client, host, httpPort, url2);
      httpCode2 = http.GET();
      Serial.print("httpCode2 : ");
      Serial.println(httpCode2);
      priBoard = board;
      url2 = "/update?api_key=MTZX12HOM1DLO4G8&field2=";
    }
    httpCode2 = 0;
  }
  
  if (priPinStatus != pinStatus) {
    while (httpCode3 != 200) {
      url3 = url3 + (String)pinStatus;
      http.begin(client, host, httpPort, url3);
      httpCode3 = http.GET();
      Serial.print("httpCode3 : ");
      Serial.println(httpCode3);
      priPinStatus = pinStatus;
      url3 = "/update?api_key=MTZX12HOM1DLO4G8&field3=";
    }
    httpCode3 = 0;
  }
  */
}
