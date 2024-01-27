#include"All_display_.hpp"
#include"RGB_.hpp"


struct KEY_TABLE{
  int index;
  int next;
  int last;
  int Return_;
  int Enter_;
  void (*operation)(void);
};

KEY_TABLE Page_table[20]={
  {0,0,0,0,1,(*bide_display)},

  {1,2,6,0,1,(*time_main_display)},
  {2,3,1,0,2,(*preweather_display)},

  

  {3,4,2,0,6,(*rgb_control_display)},
  {4,5,3,0,7,(*RGB_indy_set_display)},
  {5,6,4,0,8,(*Wifi_web_display)},
  {6,1,5,0,6,(*MPU6050_display)},

  {7,7,7,3,7,(*rgb_second_display)},
  {8,8,8,4,8,(*RGB_indy_second_display)},
  {9,9,9,5,9,(*Wifi_web_second_display)}

  
  
  
};
void bide_display();
void preweather_display();
void time_main_display();
void rgb_control_display();
//void set_info_display();
void rgb_second_display();
void RGB_indy_set_display();
void RGB_indy_second_display();
void Wifi_web_display();
void Wifi_web_second_display();
void MPU6050_display();
void (*current)();

/*------------------------main-----------------------*/


int display_flag=0;
void Menu_Choise(){
  if(ScanKey(B_M_)==1){
    display_flag=Page_table[display_flag].Enter_;
  }
   if(ScanKey(B_R_)==1){
    display_flag=Page_table[display_flag].next;
  }
  if(ScanKey(B_L_)==1){
    display_flag=Page_table[display_flag].last;
  }
   if(ScanKey(B_L_)==1&&ScanKey(B_R_)==1){
    display_flag=Page_table[display_flag].Return_;
  }
  current=Page_table[display_flag].operation;
  (*current)();
}

void setup() {
  Serial.begin(9600);
  OLED_Init();
  RGB_init();
  EEPROM.begin(1024);
  Init_mpu6050();
  now_weather_quest(); 
  pre_weather_quest();
  time_quest();
}

//60毫秒+1
void loop() {
  Menu_Choise();
  if(!rgb_isopen){
    _RGB_.clear();
    _RGB_.show();
  }
  else
    RGB_conduct();
}
