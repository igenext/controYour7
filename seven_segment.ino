#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "";// place your local network SSID
const char* pass = "";// place password
int  pin[] = {D1,D2,D3,D4,D5,D6,D7};
int num[][7] = {{1,1,1,1,1,1,0},
                {0,1,1,0,0,0,0},
                {1,1,0,1,1,0,1},
                {1,1,1,1,0,0,1},
                {0,1,1,0,0,1,1},
                {1,0,1,1,0,1,1},
                {1,0,1,1,1,1,1},
                {1,1,1,0,0,0,0},
                {1,1,1,1,1,1,1},
                {1,1,1,0,0,1,1},
                {1,1,1,0,1,1,1},
                {0,0,1,1,1,1,1},
                {1,0,0,1,1,1,0},
                {0,1,1,1,1,0,1},
                {1,0,0,1,1,1,1},
                {1,0,0,0,1,1,1}
                };

ESP8266WebServer server(80);

StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
JsonObject& payload = JSONbuffer.createObject(); 
String payloadmsg;

int count = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){ delay(1000); Serial.println("Connecting");}
  for(int i=0;i<sizeof(pin)/sizeof(pin[0]);i++){
    pinMode(pin[i], OUTPUT);
  }
  server.on("/",sync);
  server.on("/inc", inc);
  server.on("/dec", dec);
  server.begin();
}

void loop() {
   // put your main code here, to run repeatedly:
  server.handleClient();
  for(int j=0;j<sizeof(num[0])/sizeof(num[0][0]);j++){
    digitalWrite(pin[j],num[count][j]);
  }
  Serial.print("Requets can be made at ");
  Serial.print(WiFi.localIP());
  Serial.print("/ \n");    
}
void inc(){
  payloadmsg="";
  if(count == 15) count = 0;
  else count++;
  payload["value"] = count;
  payload.printTo(payloadmsg);
  server.send(200, "application/json", payloadmsg);

}

void dec(){
  payloadmsg="";
  if(count == 0) count = 0;
  else count--;
  payload["value"] = count;
  payload.printTo(payloadmsg);
  server.send(200, "application/json", payloadmsg);

}

void sync(){
  payloadmsg="";
  payload["value"] = count;
  payload.printTo(payloadmsg);
  server.send(200, "application/json", payloadmsg);
}
