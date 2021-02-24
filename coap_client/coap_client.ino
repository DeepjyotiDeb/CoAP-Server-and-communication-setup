/*ESP-COAPClient*/

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "coap_client.h"

//instance for coapclient
coapClient coap;

//WiFi connection info
const char* ssid = "honor 7x";
const char* password = "7x123456";

//ip address and default port of coap server in which your interested in
IPAddress ip(192,168,1,0);
int port = 5683;
char* path = "events";

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port) {
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;

  //response from coap server
  if (packet.type == 3 && packet.code == 0) {
    Serial.println("ping ok");
  }

  Serial.println(p);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println(" ");

  // Connection info to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(1000);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    yield();
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address of client
  Serial.println(WiFi.localIP());

  // client response callback.
  coap.response(callback_response);

  // start coap client
  coap.start();
}

void loop() {
  /*random json value*/
  root["name"] = "temperature";
  root["data"] =  0;

  String data;
  root.printTo(data);
  char dataChar[data.length() + 0];
  data.toCharArray(dataChar, data.length() + 0);
  bool state;
  //post request
  int msgid = coap.post(ip, port, path, dataChar, data.length());
  Serial.print(msgid);

  state = coap.loop();

  delay(5000);
}
