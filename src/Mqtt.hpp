#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "base.h"
#include "DHT11.hpp"

// mqttçĺ§çćˇâäź-------------------------------------------------------------------------
// çšć°ŤçŽmqttçšăĄĺçťîîŽçďż˝
WiFiClient wifiClient;             // éćśçźçźć ç˛śćŠç´ĺ¸´çšăĄĺçťďż?
MqttClient mqttClient(wifiClient); // éŤć°łçšçźć ç˛śçšăĄĺçťîçšéşăĽĺąĺŻ¤ç°qttćŠç´ĺ¸´çšăĄĺçťďż?

// çă¤ć§topoic
const char inTopic[] = "/sys/k0lufrsFejq/8266_1/thing/service/property/set";
// éćçŤˇtopic
const char outTopic[] = "/sys/k0lufrsFejq/8266_1/thing/event/property/post";
// éćĺŻćŁ°ć¨şĺ˝éŤä˝şćŽĺ¨ĺ äź
String payload_L = "{\"params\": {\"RGB_isopen\": 0},\"version\": \"1.0\"}";
String payload_T = "{\"params\": {\"Temperature\": 32},\"version\": \"1.0\"}";
String payload_H = "{\"params\": {\"Humidity\": 32},\"version\": \"1.0\"}";
String payload;

// éćśçźćśâŹćśç¨sonéĺ¨ăçľçĄč?
StaticJsonDocument<1024> doc;

void onMqttMessage(int messageSize);
void ON_subscribe();

void MQTT_connect()
{
  // mqttćŠç´ĺ¸´çšăĄĺçťç d
  mqttClient.setId(clientId);
  // mqtt ćŠç´ĺ¸´é˘ăŚĺéĺśâŹä˝¸çéŽďż?
  mqttClient.setUsernamePassword(username, passwd);
  // mqttćŠç´ĺ¸´éťćŽăéĺąžĺ˘ŚéćçšéşăĽćš´é§â?
  Serial.print("try to link mqtt sever");
  Serial.println(broker);
  int l = 0;
  // ćŠç´ĺ¸´mqttéĺśĺ§éŁďż?
  if (!mqttClient.connect(broker, port))
  {
    Serial.print("mqtt sever connect error");
    Serial.println(mqttClient.connectError());
  }
  else
  { // ćŠç´ĺ¸´mqttéĺśĺ§éŁăŚĺéç¸ĺ˝çťďż?
    Serial.println("successfully connect mqtt sever");
    Serial.println();
    ON_subscribe();
  }
}

void ON_subscribe()
{
  // çĺ§çĺ¨ĺ äźéşăŚćšéĽçśç
  mqttClient.onMessage(onMqttMessage);

  // éľćłĺľçă¤ć§ćśĺŠî˝ćˇâäź
  Serial.print("sub topic: ");
  Serial.println(inTopic);
  Serial.println();

  // mqttçă¤ć§ćśĺŠî˝
  mqttClient.subscribe(inTopic);
}

void send_MqttMessage()
{

  mqttClient.poll();
  Serial.print("send with topic: ");
  Serial.println(outTopic);

  DynamicJsonDocument json_msg(512);
  DynamicJsonDocument json_data(512);

  json_data["Temperature"] = DHT11.temperature;
  json_data["Humidity"] = DHT11.humidity;
  
  json_msg["params"] = json_data;
  json_msg["version"] = "1.0";

  serializeJson(json_msg,payload);

  Serial.println(payload);

  mqttClient.beginMessage(outTopic, payload.length(), false, 1, false); // éĺŤçźĺ¨ĺ äźçťîĺ˝?
  mqttClient.print(payload);                                            // éćŚâŹä˝šç§ˇé­ďż?
  mqttClient.endMessage();                                              // éć˝ćŁ´çťîĺ˝
}

void onMqttMessage(int messageSize)
{
  // çĺĺ¸´éčˇşĺé¨ĺŻsonéçĺľćîç¨éăĽĺéćŽç˛é˛ďż?
  char ch_mqtt[messageSize];
  for (int i = 0; i < messageSize; i++)
  {
    ch_mqtt[i] = mqttClient.read();
  }

  // éľćłĺľéşăŚćšéćŽćŽéçĺľćžśŃçŹ
  Serial.print("receve message size:messageSize=");
  Serial.print(messageSize);
  Serial.println();

  // éľćłĺľćîĺ´˛éĺşŁćŽéćŽç˛éĺ­î?
 /* Serial.print("message context:[messageSize]=");
  Serial.print(ch_mqtt);
  Serial.println();

  // çď˝ç˝jsonéçĺľ?
  deserializeJson(doc, ch_mqtt);

  // çĺEDSwitchéĺŽanSwatchćŠćśîç§ĺŹŞâŹďż?
  bool led = doc["params"]["RGB_isopen"];
  Serial.print("led=");
  Serial.print(led);
  Serial.println();

  bool fan = doc["params"]["fan_isopen"];
  Serial.print("fan=");
  Serial.print(fan);
  Serial.println();*/

  // ˝âÎö JSON ×Öˇű´Ž  
  DeserializationError error = deserializeJson(doc, ch_mqtt);  
  
  // źě˛é˝âÎöĘÇˇńłÉšŚ  
  if (error) {  
    Serial.print("deserializeJson() failed: ");  
    Serial.println(error.c_str());  
    return;  
  }  
  
  // ťńČĄ items śÔĎó  
  JsonObject items = doc["items"];  
  
  
  int fan,led;

  // ĚáČĄ RGB_isopen ľÄ value  
  if (items.containsKey("RGB_isopen")) {  
    JsonObject rgbIsopen = items["RGB_isopen"];  
    led = rgbIsopen["value"];  
    Serial.print("RGB_isopen value: ");  
    Serial.println(led);  
     if (led == 1)
  {
    Serial.println("LED_ON");
    Serial.println();
    rgb_isopen=1;
  }
  else if (led == 0)
  {
    Serial.println("LED_OFF");
    Serial.println();
    rgb_isopen=0;
  }
  }
  
  // ĚáČĄ fan_isopen ľÄ value  
  if (items.containsKey("fan_isopen")) {  
    JsonObject fanIsopen = items["fan_isopen"];  
    fan = fanIsopen["value"];  
    Serial.print("fan_isopen value: ");  
    Serial.println(fan);  
      if (fan == 1)
  {
    Serial.println("fan_ON");
    Serial.println();
    fan_isopen=1;
  }
  else if (fan == 0)
  {
    Serial.println("fan_OFF");
    Serial.println();
    fan_isopen=0;
  }
  }  
  
}

void log_out_mqtt()
{
  mqttClient.stop();
}