/*ESP-COAP Server*/

#include <ESP8266WiFi.h>
#include <coap_server.h>

// CoAP server endpoint url callback
void callback_light(coapPacket &packet, IPAddress ip, int port, int obs);

coapServer coap;
const char* ssid="Esp82";
const char* pass="12345678";
bool LEDSTATE;

// CoAP server endpoint URL
void callback_light(coapPacket *packet, IPAddress ip, int port,int obs) 
{
  // send response
  char p[packet->payloadlen + 1];
  memcpy(p, packet->payload, packet->payloadlen);
  p[packet->payloadlen] = NULL;
  Serial.println(p);

  String message(p);

  if (message.equals("0"))
  {
    digitalWrite(16,LOW);
    Serial.println("if loop");
  }
  else
  {
    digitalWrite(16,HIGH);
    Serial.println("else loop");
  } 
  char *light = (digitalRead(16) > 0)? ((char *) "1") :((char *) "0");
  
   //coap.sendResponse(packet, ip, port, light);
   if(obs==1)
    coap.sendResponse(light);
   else
    coap.sendResponse(ip,port,light);
}
void callback_lightled(coapPacket *packet, IPAddress ip, int port,int obs) {
  Serial.println("Lightled");
  // send response
  char p[packet->payloadlen + 1];
  memcpy(p, packet->payload, packet->payloadlen);
  p[packet->payloadlen] = NULL;

  String message(p);

  if (message.equals("0"))
    LEDSTATE = false;
  else if (message.equals("1"))
    LEDSTATE = true;

//  if (LEDSTATE) {
//    digitalWrite(5, HIGH) ;
//    if(obs==1)
//     coap.sendResponse("1");
//     else
//    coap.sendResponse(ip, port, "1");
//    
//    //coap.sendResponse("1");
//  } else {
//    digitalWrite(5, LOW) ;
//    if (obs==1)
//    coap.sendResponse("0");
//    else
//    coap.sendResponse(ip, port, "0");
//    //coap.sendResponse("0");
//  }
}


void setup() {
  yield();
  //serial begin
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,pass,1);
  Serial.println(" ");

  // start WiFi hotspot
  Serial.println("starting");
  Serial.println();
  Serial.println("IP  :");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());

  // LED State
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  LEDSTATE = true;

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  coap.server(callback_light, "light");
  coap.server(callback_lightled, "lightled");
  coap.start();
}

void loop() {
  coap.loop();
  delay(1000);
}
