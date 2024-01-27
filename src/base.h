#include<Arduino.h>
#ifndef _BASE_H_
#define _BASE_H_

#define B_M_ 2
#define B_L_ 0
#define B_R_ 15
#define RGB_PIN 13
#define RGB_NUMBER 2

#define STASSID "SSR_8266"
#define STAPSK  "22222222"


//const char *Wifi_Name="ChinaNet-5P7d";
//const char *WIfi_Password="744zf67h";

String reqUserKey = "SVQ6L8nXRYiQgj-PS";   // your weather private key
String reqLocation = "GuiLin";  //  your city


bool rgb_isopen=1;

int rgb_color=2;
int rgb_lumin=50;
int rgb_type=3;
int rgm_max_lumin=50;
int breathe_speed=1;//1-10

int rgb_R=100;
int rgb_G=100;
int rgb_B=100;

//dormitory
//const char *Wifi_Name="CMCC-FPC2";
//const char *WIfi_Password="qcu5skpf";



#endif