#include"base.h"
#include<DFRobot_DHT11.h>

DFRobot_DHT11 DHT11;

int _Temperature=0;
int _Humidity=0;

void renew_T_H_data(){
  DHT11.read(DHT11_PIN);
  _Temperature=DHT11.temperature;
  _Humidity=DHT11.humidity;
  Serial.println(_Temperature);
  Serial.println(_Humidity);

}