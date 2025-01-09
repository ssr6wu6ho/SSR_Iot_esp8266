#include "base.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <DNSServer.h>

DNSServer dnsServer;

const char *ssid = STASSID;
const char *password = STAPSK;

const byte DNS_PORT = 53; // DNS端口号

byte SISSD_ADDR = 0;
byte PASSWORD_ADDR = 100;
byte TCP_ADDR = 200;
byte SISSD_NUM = 0;
byte PASSWORD_NUM = 0;
String GET_SSID;
String GET_PASSWORD;

IPAddress local_IP(192, 168, 4, 1); // 手动设置的开启的网络的ip地址
IPAddress subnet(255, 255, 255, 0); // 手动设置的子网掩码

ESP8266WebServer server(80);

String str =
    "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta name=\"viewport\"content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\"content=\"ie=edge\"><title>ESP8266_WEBnetwork</title></head><body><form name=\"my\">WIFI name:<input type=\"text\"name=\"s\"placeholder=\"WIFI name:\"id=\"aa\"><br>ENETR your WIFI password<input type=\"text\"name=\"p\"placeholder=\"ENETR your WIFI password\"id=\"bb\"><br><input type=\"button\"value=\"connect\"onclick=\"wifi()\"></form><script language=\"javascript\">function wifi(){var ssid=my.s.value;var password=bb.value;var xmlhttp=new XMLHttpRequest();xmlhttp.open(\"GET\",\"/HandleVal?ssid=\"+ssid+\"&password=\"+password,true);xmlhttp.send()}</script></body></html>";

void set_String(int a, int b, String str);
String get_String(int a, int b);
void handleRoot();
bool autoConfig();
void handleNotFound();
void HandleVal();

void Wifi_connect(int flag_a)
{
  if (flag_a == 0)
  { // connect airpartment wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(Wifi_Name, WIfi_Password);
    for (int i = 0; i < 10; i++)
    {
      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.println("Wifi conntected.");
        return;
      }
      delay(1000);
      Serial.print(".");
    }
    Serial.print("\nConnect failed: timemout!\n");
  }
  else if (flag_a == 1)
  { // EEPROM wifi
    if (EEPROM.read(SISSD_ADDR) != 0)
    {
      GET_SSID = get_String(EEPROM.read(SISSD_ADDR), SISSD_ADDR + 1);
      GET_PASSWORD = get_String(EEPROM.read(PASSWORD_ADDR), PASSWORD_ADDR + 1);
      Serial.printf("SSID:%s\r\n", GET_SSID.c_str());
      Serial.printf("PSW:%s\r\n", GET_PASSWORD.c_str());
      WiFi.mode(WIFI_STA);
      WiFi.begin(GET_SSID, GET_PASSWORD);
      for (int i = 0; i < 20; i++)
      {
        if (WiFi.status() == WL_CONNECTED)
        {
          Serial.println("Wifi conntected.");
          return;
        }
        delay(1000);
        Serial.print(".");
      }
      Serial.print("\nConnect failed: timemout!\n");
    }
  }
}

void htmlConfig()
{
  WiFi.mode(WIFI_AP_STA); // 设置模式为AP+STA
  WiFi.softAPConfig(local_IP, local_IP, subnet);

  WiFi.softAP(ssid, NULL);

  Serial.println("AP设置完成");
  server.on("/", handleRoot);
  // server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound); // 请求失败回调函数
  // server.on("/HandleVal", HTTP_POST, HandleVal);
  server.on("/HandleVal", HTTP_GET, HandleVal);
  // MDNS.begin("esp8266");
  server.begin(); // 开启服务器
  dnsServer.start(DNS_PORT, "*", local_IP);

  while (1)
  {
    server.handleClient();
    dnsServer.processNextRequest();
    if (WiFi.status() == WL_CONNECTED)
    {
      SISSD_NUM = GET_SSID.length();
      PASSWORD_NUM = GET_PASSWORD.length();
      EEPROM.write(SISSD_ADDR, SISSD_NUM);
      set_String(SISSD_NUM, SISSD_ADDR + 1, GET_SSID);
      EEPROM.write(PASSWORD_ADDR, PASSWORD_NUM);
      set_String(PASSWORD_NUM, PASSWORD_ADDR + 1, GET_PASSWORD);
      server.stop();
      break;
    }
  }
}

// request the client callback
void handleRoot()
{
  server.send(200, "text/html", str);
}

// conduct the massage from client callback
void HandleVal()
{
  GET_SSID = server.arg("ssid");         // 从JavaScript发送的数据中找ssid的值
  GET_PASSWORD = server.arg("password"); // 从JavaScript发送的数据中找password的值
  Serial.printf("SSID:%s\r\n", GET_SSID.c_str());
  Serial.printf("PWD:%s\r\n", GET_PASSWORD.c_str());
  WiFi.begin(GET_SSID, GET_PASSWORD);
}

// autoConfig_wifi
bool autoConfig()
{
  if (EEPROM.read(SISSD_ADDR) != 0)
  {

    GET_SSID = get_String(EEPROM.read(SISSD_ADDR), SISSD_ADDR + 1);
    GET_PASSWORD = get_String(EEPROM.read(PASSWORD_ADDR), PASSWORD_ADDR + 1);
    Serial.printf("SSID:%s\r\n", GET_SSID.c_str());
    Serial.printf("PSW:%s\r\n", GET_PASSWORD.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.begin(GET_SSID, GET_PASSWORD);

    WiFi.mode(WIFI_STA);
    WiFi.begin();
    Serial.print("AutoConfig Waiting......");
    for (int i = 0; i < 10; i++)
    {
      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.println("AutoConfig Success");
        Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
        Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
        // WiFi.printDiag(Serial);
        return true;
        // break;
      }
      else
      {
        Serial.print(".");
        delay(1000);
      }
    }
    Serial.println("AutoConfig Faild!");
  }
  return false;
}

// request Faild
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

// a写入字符串长度，b是起始位，str为要保存的字符串
void set_String(int a, int b, String str)
{
  EEPROM.write(a, str.length()); // EEPROM第a位，写入str字符串的长度
  // 把str所有数据逐个保存在EEPROM
  for (int i = 0; i < str.length(); i++)
  {
    EEPROM.write(b + i, str[i]);
  }
  EEPROM.commit();
}

// a位是字符串长度，b是起始位
String get_String(int a, int b)
{
  String data = "";
  // 从EEPROM中逐个取出每一位的值，并链接
  for (int i = 0; i < a; i++)
  {
    data += char(EEPROM.read(b + i));
  }
  return data;
}
