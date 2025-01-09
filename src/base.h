#include <Arduino.h>
#include <Wire.h>
#ifndef _BASE_H_
#define _BASE_H_

#define B_M_ 2
#define B_L_ 15
#define B_R_ 0
#define RGB_PIN 13
#define DHT11_PIN 12
#define JDQ_PIN 14
#define RGB_NUMBER 4


#define STASSID "SSR_8266"
#define STAPSK "22222222"

// const char *Wifi_Name="ChinaNet-5P7d";
// const char *WIfi_Password="744zf67h";

const char *Wifi_Name = "CMCC-FPC2";
const char *WIfi_Password = "qcu5skpf";

String reqUserKey = "SVQ6L8nXRYiQgj-PS"; // your weather private key
String reqLocation = "DaLian";           //  your city

// Mqtt__ALY_set
const char broker[] = "iot-06z00bdg6nkynic.mqtt.iothub.aliyuncs.com";
// 端口号
int port = 1883;
String clientId = "k0lufrsFejq.8266_1|securemode=2,signmethod=hmacsha256,timestamp=1710552974327|";
String username = "8266_1&k0lufrsFejq";
String passwd = "d827832e9b6c48c077658363f86817ff5eefc772ca3f8f46e465fcbe74d49fa7";

bool rgb_isopen = 1;
bool fan_isopen = 1;

int rgb_color = 2;
int rgb_lumin = 50;
int rgb_type = 3;
int rgm_max_lumin = 50;
int breathe_speed = 1; // 1-10

int rgb_R = 100;
int rgb_G = 100;
int rgb_B = 100;



// dormitory
// const char *Wifi_Name="CMCC-FPC2";
// const char *WIfi_Password="qcu5skpf";

#endif