#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "base.h"

void now_weather_quest();
void pre_weather_quest();
void esp_http_quest(String reqRes, int i);
void JsonInfo_now(WiFiClient client);
void JsonInfo_pre(WiFiClient client);
void time_quest();

String reqUnit = "c"; // C/T

const char *host = "api.seniverse.com"; // link weathers IP
const int httpPort = 80;                // link weathers port

// before_update weather
String s_old_weather;
String s_old_daily;
int s_old_temperature;
int s_old_weather_type;

String result0_Tomorrow_w_date;
String result0_Tomorrow_w_text_day;
int result0_Tomorrow_w_code_day;
int result0_Tomorrow_w_high;
int result0_Tomorrow_w_low;

String result0_After_Tomorrow_w_date;
String result0_After_Tomorrow_w_text_day;
int result0_After_Tomorrow_w_code_day;
int result0_After_Tomorrow_w_high;
int result0_After_Tomorrow_w_low;

// before_update time
String All_Time;
String Hours;
String Minutes;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// request current weather info
void now_weather_quest()
{
  // ???????????API?????????????????
  String reqRes = "/v3/weather/now.json?key=" + reqUserKey +
                  +"&location=" + reqLocation +
                  "&language=en&unit=" + reqUnit;
  esp_http_quest(reqRes, 1); // 1_type current
}
// request pre weather info
void pre_weather_quest()
{
  String reqRes = "/v3/weather/daily.json?key=" + reqUserKey +
                  +"&location=" + reqLocation + "&language=en&unit=" +
                  reqUnit + "&start=0&days=3";
  esp_http_quest(reqRes, 2);
}

void esp_http_quest(String reqRes, int i)
{

  WiFiClient client;

  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n";

  if (client.connect(host, 80))
  {
    // ?????????????
    client.print(httpRequest);

    // ???????????http???????
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);

    // ?????????????????????
    if (client.find("\r\n\r\n"))
    {
      Serial.println("Found Header End. Start Parsing.");
    }

    // ????ArduinoJson??????????????????
    if (i == 1)
      JsonInfo_now(client);
    else
      JsonInfo_pre(client);

    client.stop();
  }
}
// request pre weather info
void JsonInfo_now(WiFiClient client)
{
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, client);
  // Serial.println(doc.as<String>());

  JsonObject result0 = doc["results"][0];
  JsonObject result0_now = result0["now"];

  String g_strWeather = result0_now["text"].as<String>();       // "Sunny"
  int g_iCode = result0_now["code"].as<int>();                  // "0"
  int g_iTemperature = result0_now["temperature"].as<int>();    // "32"
  String g_strUpdateTime = result0["last_update"].as<String>(); // "2020-06-02T14:40:00+08:00"

  Serial.println(F("======Weahter Now======="));
  Serial.print(F("Weather Now: "));
  Serial.print(g_strWeather);
  Serial.print(F(" -> "));
  Serial.println(g_iCode);
  Serial.print(F("Temperature: "));
  Serial.println(g_iTemperature);
  Serial.print(F("Last Update: "));
  Serial.println(g_strUpdateTime);
  Serial.println(F("========================"));

  s_old_weather = g_strWeather;
  s_old_temperature = g_iTemperature;
  s_old_daily = g_strUpdateTime.substring(0, g_strUpdateTime.indexOf("T"));
  s_old_weather_type = g_iCode;
}

void JsonInfo_pre(WiFiClient client)
{

  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 3 * JSON_OBJECT_SIZE(14) + 860;

  DynamicJsonDocument doc(capacity);

  deserializeJson(doc, client);

  JsonObject results_0 = doc["results"][0];

  JsonArray results_0_daily = results_0["daily"];
  // 0:today;1:Tomorrow;2:the day after Tomorrow
  JsonObject result0_Tomorrow_w = results_0_daily[1];

  JsonObject result0_After_Tomorrow_w = results_0_daily[2];

  result0_Tomorrow_w_date = result0_Tomorrow_w["date"].as<String>();
  result0_Tomorrow_w_text_day = result0_Tomorrow_w["text_day"].as<String>();
  result0_Tomorrow_w_code_day = result0_Tomorrow_w["code_day"].as<int>();
  result0_Tomorrow_w_high = result0_Tomorrow_w["high"].as<int>();
  result0_Tomorrow_w_low = result0_Tomorrow_w["low"].as<int>();

  result0_After_Tomorrow_w_date = result0_After_Tomorrow_w["date"].as<String>();
  result0_After_Tomorrow_w_text_day = result0_After_Tomorrow_w["text_day"].as<String>();
  result0_After_Tomorrow_w_code_day = result0_After_Tomorrow_w["code_day"].as<int>();
  result0_After_Tomorrow_w_high = result0_After_Tomorrow_w["high"].as<int>();
  result0_After_Tomorrow_w_low = result0_After_Tomorrow_w["low"].as<int>();

  Serial.println(F("======Tomorrow Weahter ======="));
  Serial.print(F("DATE: "));
  Serial.println(result0_Tomorrow_w_date);
  Serial.print(F("Day Weather: "));
  Serial.print(result0_Tomorrow_w_text_day);
  Serial.print(F(" "));
  Serial.println(result0_Tomorrow_w_code_day);
  Serial.print(F("High: "));
  Serial.println(result0_Tomorrow_w_high);
  Serial.print(F("LOW: "));
  Serial.println(result0_Tomorrow_w_low);
  Serial.println(F("=============================="));

  Serial.println(F("======After Tomorrow Weahter ======="));
  Serial.print(F("DATE: "));
  Serial.println(result0_After_Tomorrow_w_date);
  Serial.print(F("Day Weather: "));
  Serial.print(result0_After_Tomorrow_w_text_day);
  Serial.print(F(" "));
  Serial.println(result0_After_Tomorrow_w_code_day);
  Serial.print(F("High: "));
  Serial.println(result0_After_Tomorrow_w_high);
  Serial.print(F("LOW: "));
  Serial.println(result0_After_Tomorrow_w_low);
  Serial.println(F("=============================="));
}

/*----------------quest time info------------------------*/

void time_quest()
{
  timeClient.update();
  timeClient.setTimeOffset(28800); 
  Serial.println(timeClient.getFormattedTime());
  All_Time = timeClient.getFormattedTime();
  Hours = All_Time.substring(0, All_Time.indexOf(":"));
  Minutes = All_Time.substring(All_Time.indexOf(":") + 1, All_Time.lastIndexOf(":"));
}
