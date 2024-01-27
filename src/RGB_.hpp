#include<Adafruit_NeoPixel.h>
#include"base.h"
Adafruit_NeoPixel _RGB_ = Adafruit_NeoPixel(RGB_NUMBER, RGB_PIN, NEO_GRB + NEO_KHZ800);

void RGB_init();
void colorWipe(uint32_t c, uint8_t wait);
void rainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos);
void RGB_cooperate();

/*------------------------------RGB_conduct----------------*/
//1:normal 2:breathe
int update_color=1;
int update_rainbow=1;
bool isbreathe_off=false; //0-100

void RGB_conduct(){
  _RGB_.setBrightness(rgb_lumin);
  if(rgb_type==1){
    for(int i=0;i<RGB_NUMBER;i++){ 
      _RGB_.setPixelColor(i,rgb_R,rgb_G,rgb_B);
    }
  }

  else if(rgb_type==2){
    for(int i=0;i<RGB_NUMBER;i++){ 
      _RGB_.setPixelColor(i,rgb_R,rgb_G,rgb_B);
    }
     if(isbreathe_off){
      rgb_lumin+=breathe_speed;
      if(rgb_lumin>=rgm_max_lumin){
        rgb_lumin=rgm_max_lumin-2;
        isbreathe_off=!isbreathe_off;
      }
    }
    else if(!isbreathe_off){
     rgb_lumin-=breathe_speed;
      if(rgb_lumin<=1){
        rgb_lumin=2;
        isbreathe_off=!isbreathe_off;
      }
    }
  }

  else if(rgb_type==3){
    for(int i=0;i<RGB_NUMBER;i++){ 
      _RGB_.setPixelColor(i,rgb_R,rgb_G,rgb_B);
    }
     if(isbreathe_off){
      rgb_lumin+=(breathe_speed*5);
      if(rgb_lumin>=rgm_max_lumin){
        rgb_lumin=rgm_max_lumin-2;
        isbreathe_off=!isbreathe_off;
      }
    }
    else if(!isbreathe_off){
     rgb_lumin-=(breathe_speed*5);
      if(rgb_lumin<=1){
        rgb_lumin=2;
        isbreathe_off=!isbreathe_off;
        rgb_R=random(0,254);
        rgb_G=random(0,254);
        rgb_B=random(0,254);
      }
    }
  }

  _RGB_.show();
}


/*----------------------------RGB-----------------------*/
void RGB_init(){
  _RGB_.begin(); 
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<_RGB_.numPixels(); i++) {
    _RGB_.setPixelColor(i, c);
    _RGB_.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<_RGB_.numPixels(); i++) {
      _RGB_.setPixelColor(i, Wheel((i+j) & 255));
    }
    _RGB_.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return _RGB_.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return _RGB_.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return _RGB_.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
