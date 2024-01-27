#include"OLED_.hpp"
#include"base.h"  
#include"weaher_time_.hpp"
#include"Wifi_web.hpp"
#include"mpu6050_.hpp"

//button_scan
int ScanKey(int Button){         
  
    //unsigned long currentTime=millis();


    if(Button==B_R_){
      if(digitalRead(Button)==1){  //button_R is pull down
      delay(50);
      if(digitalRead(Button)==1){
        return 1;
        }
      }
    }
    else{
      if(digitalRead(Button)==0){  
      delay(50);
      if(digitalRead(Button)==0){
        return 1;
        }
      }
    }

  return 0;
}

/*----------------display_weather_time------------------------------*/


int Weather_update_flag=1;
int Time_update_flag=1;
int frame_s = 0;

void display_weather_time_info(){
  //OLED_ShowString(85,2,1,reqLocation);    
  OLED_ShowString(2,55,1,s_old_weather);  
  OLED_ShowString(2,45,1,"T:");
  OLED_ShowNum(12,45,1,s_old_temperature); 
  OLED_ShowString(2,2,1,s_old_daily); 
  
  OLED_ShowString(30,25,2,Hours);
  OLED_ShowString(60,25,2,Minutes);
  
  oled.drawRoundRect(86,0,120,30,70,WHITE);
  OLED_ShowString(2,14,1,"[SSR]");
  oled.drawFastHLine(28,40,55,WHITE);
}

void display_pre_weather(int weather_type,int x,int y){
  if(weather_type==1){
    OLED_ShowString(x+10,y,1,"D:");
    OLED_ShowString(x+20,y,1,result0_Tomorrow_w_date.substring(result0_Tomorrow_w_date.length()-2));
    OLED_ShowString(x+5,y+10+32,1,result0_Tomorrow_w_text_day);
    OLED_ShowNum(x+5,y+20+32,1,result0_Tomorrow_w_low);
    OLED_ShowString(x+18,y+20+32,1,"-");
    OLED_ShowNum(x+25,y+20+32,1,result0_Tomorrow_w_high);

    if(result0_Tomorrow_w_code_day==6||result0_Tomorrow_w_code_day==7||result0_Tomorrow_w_code_day==8||result0_Tomorrow_w_code_day==4){
    oled.drawBitmap(x+5, y+10, f_cloudy[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(result0_Tomorrow_w_code_day==0||result0_Tomorrow_w_code_day==1){
    oled.drawBitmap(x+5, y+10, f_sum[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(result0_Tomorrow_w_code_day==19||result0_Tomorrow_w_code_day==20||result0_Tomorrow_w_code_day==21||result0_Tomorrow_w_code_day==22||result0_Tomorrow_w_code_day==23||result0_Tomorrow_w_code_day==24||result0_Tomorrow_w_code_day==25){
    oled.drawBitmap(x+5, y+10, f_snow[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(result0_Tomorrow_w_code_day==10||result0_Tomorrow_w_code_day==11||result0_Tomorrow_w_code_day==12||result0_Tomorrow_w_code_day==13||result0_Tomorrow_w_code_day==14||result0_Tomorrow_w_code_day==15||result0_Tomorrow_w_code_day==16||result0_Tomorrow_w_code_day==17||result0_Tomorrow_w_code_day==18){
    oled.drawBitmap(x+5, y+10, f_rain[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(result0_Tomorrow_w_code_day==9)
    oled.drawBitmap(x+5, y+10, f_overcast[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  
  }

  else{
    OLED_ShowString(x+10,y,1,"D:");
    OLED_ShowString(x+20,y,1,result0_After_Tomorrow_w_date.substring(result0_After_Tomorrow_w_date.length()-2));
    OLED_ShowString(x+5,y+10+32,1,result0_After_Tomorrow_w_text_day);
    OLED_ShowNum(x+5,y+20+32,1,result0_After_Tomorrow_w_low);
    OLED_ShowString(x+18,y+20+32,1,"-");
    OLED_ShowNum(x+25,y+20+32,1,result0_After_Tomorrow_w_high); 

      if(result0_After_Tomorrow_w_code_day==6||result0_After_Tomorrow_w_code_day==7||result0_After_Tomorrow_w_code_day==8||result0_After_Tomorrow_w_code_day==4){
    oled.drawBitmap(x+5, y+10, f_cloudy[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(result0_After_Tomorrow_w_code_day==0||result0_After_Tomorrow_w_code_day==1){
    oled.drawBitmap(x+5, y+10, f_sum[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(result0_After_Tomorrow_w_code_day==19||result0_After_Tomorrow_w_code_day==20||result0_After_Tomorrow_w_code_day==21||result0_After_Tomorrow_w_code_day==22||result0_After_Tomorrow_w_code_day==23||result0_After_Tomorrow_w_code_day==24||result0_After_Tomorrow_w_code_day==25){
    oled.drawBitmap(x+5, y+10, f_snow[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(result0_After_Tomorrow_w_code_day==10||result0_After_Tomorrow_w_code_day==11||result0_After_Tomorrow_w_code_day==12||result0_After_Tomorrow_w_code_day==13||result0_After_Tomorrow_w_code_day==14||result0_After_Tomorrow_w_code_day==15||result0_After_Tomorrow_w_code_day==16||result0_After_Tomorrow_w_code_day==17||result0_After_Tomorrow_w_code_day==18){
    oled.drawBitmap(x+5, y+10, f_rain[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(result0_After_Tomorrow_w_code_day==9)
    oled.drawBitmap(x+5, y+10, f_overcast[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  
  }
  
  frame_s = (frame_s + 1) % FRAME_COUNT_32;
}

void time_main_display(){
  oled.clearDisplay();
  display_weather_time_info();
  oled.drawBitmap(90, 32, frames_skate_board[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  if(Weather_update_flag%9==0){
    OLED_ShowString(50,25,2,":");
  }

  if(s_old_weather_type==6||s_old_weather_type==7||s_old_weather_type==8||s_old_weather_type==4){
    oled.drawBitmap(90, 0, f_cloudy[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(s_old_weather_type==0||s_old_weather_type==1){
    oled.drawBitmap(90, 0, f_sum[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(s_old_weather_type==19||s_old_weather_type==20||s_old_weather_type==21||s_old_weather_type==22||s_old_weather_type==23||s_old_weather_type==24||s_old_weather_type==25){
    oled.drawBitmap(90, 0, f_snow[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(s_old_weather_type==10||s_old_weather_type==11||s_old_weather_type==12||s_old_weather_type==13||s_old_weather_type==14||s_old_weather_type==15||s_old_weather_type==16||s_old_weather_type==17||s_old_weather_type==18){
    oled.drawBitmap(90, 0, f_rain[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  else if(s_old_weather_type==9){
    oled.drawBitmap(90, 0, f_overcast[frame_s], FRAME_WIDTH, FRAME_HEIGHT, 1);
  }
  
  oled.display();
  frame_s = (frame_s + 1) % FRAME_COUNT_32;    //����ͼ��

  Time_update_flag++;
  if(Time_update_flag==150){ //30s update time
    
    time_quest();
    Time_update_flag=0;
  }
  delay(40);
}

void preweather_display(){
  oled.clearDisplay();
  display_pre_weather(1,0,0);//1 type tomorrow,2:after tomorrow
  display_pre_weather(2,60,0);
  oled.display();
  delay(40);
}

/*----------------------bide_display----------------------------------*/

int emoji_index=1;
void bide_display(){
   oled.clearDisplay();
   if(ScanKey(B_R_)==1||ScanKey(B_L_)==1){
    emoji_index++;
   }
  switch(emoji_index){
    case 0:
      emoji_index=4;
      break;
    case 1:
      oled.drawBitmap(32, 0, f_skateboard_64[frame_s], 64, 64, 1);
      break;
    case 2:
      oled.drawBitmap(32, 0, f_cat[frame_s], 64, 64, 1);
      break;
    case 3:
      oled.drawBitmap(32, 0, f_ghost[frame_s], 64, 64, 1);
      break;
    case 4:
      oled.drawBitmap(32, 0, f_face_love[frame_s], 64, 64, 1);
      break;
    case 5:
      emoji_index=1;
      break;
  }
  frame_s = (frame_s + 1) % FRAME_COUNT_64;
  oled.display();
  delay(40);
}

/*----------------------------RGB_contron----------------------------*/

void rgb_control_display(){
  oled.clearDisplay();
  oled.drawBitmap(42, 5, f_rgb[frame_s], FRAME_WIDTH_48, FRAME_HEIGHT_48, 1);
  oled.drawRect(0, 0, 128, 64, 1);
  OLED_ShowString(4,53,1,"RGB_SET"); 
  oled.display();
  frame_s = (frame_s + 1) % FRAME_COUNT_48;
  delay(40);
}

int rgb_index=1;
bool rgb_iscolor=false,rgb_islumin=false,rgb_istype=false,rgb_isspeed=false;

void rgb_second_display(){
  oled.clearDisplay();
  oled.drawRect(0, 0, 128, 64, 1);
  oled.drawRect(2, 2, 124, 60, 1);

  if(!rgb_iscolor&&!rgb_islumin&&!rgb_istype&&!rgb_isspeed){
  //button_choise
    if(ScanKey(B_R_)==1)
      rgb_index++;
    if(ScanKey(B_L_)==1)
      rgb_index--;
    //draw
    switch (rgb_index)
    {
    case 0:
      rgb_index=5;
      break;
    case 1:
      oled.drawRect(4,10,125,10,1);
      if(ScanKey(B_M_)==1){
        delay(100);
        rgb_isopen=!rgb_isopen;
        }
      break;
    case 2:
      oled.drawRect(4,20,125,10,1);
      if(ScanKey(B_M_)==1)
        rgb_iscolor=true;
      break;
    case 3:
      oled.drawRect(4,30,125,10,1);
      if(ScanKey(B_M_)==1)
        rgb_islumin=true;
      break;
    case 4:
      oled.drawRect(4,40,125,10,1);
      if(ScanKey(B_M_)==1)
        rgb_istype=true;
      break;
    case 5:
     oled.drawRect(4,50,125,10,1);
      if(ScanKey(B_M_)==1)
        rgb_isspeed=true;
      break;
    case 6:
      rgb_index=1;
      break;
    }
    if(rgb_isopen)
      OLED_ShowString(5,12,1,"ON");
    else
      OLED_ShowString(5,12,1,"OFF");

    OLED_ShowString(5,22,1,"COLOR");
    OLED_ShowString(5,32,1,"LUMINANCE");
    OLED_ShowString(5,42,1,"TYPE");
    OLED_ShowString(5,52,1,"SPEED");
    
  }

  if(rgb_iscolor){
    delay(100);
    OLED_ShowString(10,6,2,"COLOR");
    if(ScanKey(B_M_)==1){
        rgb_iscolor=false;
      }
      if(ScanKey(B_R_)==1)
      rgb_color++;
      if(ScanKey(B_L_)==1)
      rgb_color--;
      switch(rgb_color){
        case 0:
          rgb_color=1;
        case 1:
          OLED_ShowString(55,30,1,"random");
          rgb_R=169,rgb_G=0,rgb_B=56;
          break;
        case 2:
          OLED_ShowString(55,30,1,"warm");
          rgb_R=169,rgb_G=101,rgb_B=56;
          break;
        case 3:
          OLED_ShowString(55,30,1,"white");
          rgb_R=100,rgb_G=100,rgb_B=100;
          break;
        case 4:
          rgb_color=0;
          break;
      }
  }

  else if(rgb_islumin){
      delay(100);
      OLED_ShowString(10,6,2,"LUMINANCE");
      if(ScanKey(B_M_)==1){
        rgb_islumin=false;
      }
      if(ScanKey(B_R_)==1)
      rgm_max_lumin+=10;
      if(ScanKey(B_L_)==1)
      rgm_max_lumin-=10;
      if(rgm_max_lumin>=231)
        rgm_max_lumin=1;
      if(rgm_max_lumin<=0)
        rgm_max_lumin=229;
      oled.fillRect(10,25,int(rgm_max_lumin/2)-10,20,1);
      rgb_lumin=rgm_max_lumin;
  }

  else if(rgb_istype){
    delay(100);
      OLED_ShowString(10,6,2,"TYPE");
      if(ScanKey(B_M_)==1){
        rgb_istype=false;
      }
      if(ScanKey(B_R_)==1)
      rgb_type++;
      if(ScanKey(B_L_)==1)
      rgb_type--;
      switch(rgb_type){
        case 0:
          rgb_type=1;
        case 1:
          OLED_ShowString(55,30,1,"normal");
          break;
        case 2:
          OLED_ShowString(55,30,1,"breathe");
          break;
        case 3:
          OLED_ShowString(55,30,1,"rainbow");
          break;
        case 4:
          rgb_type=0;
          break;
      }
  }
  
  else if(rgb_isspeed){
    delay(100);
      OLED_ShowString(10,6,2,"SPEED");
      if(ScanKey(B_M_)==1){
        rgb_isspeed=false;
      }
      if(ScanKey(B_R_)==1)
      breathe_speed++;
      if(ScanKey(B_L_)==1)
      breathe_speed--;
      if(breathe_speed>=11)
        breathe_speed=1;
      if(breathe_speed<=0)
        breathe_speed=10;
      oled.fillRect(10,25,int(breathe_speed*10),20,1);
  }
  oled.display();
  delay(40);
}

void RGB_indy_set_display(){
  oled.clearDisplay();
  oled.drawBitmap(42, 5, f_rgb[frame_s], FRAME_WIDTH_48, FRAME_HEIGHT_48, 1);
  oled.drawRect(0, 0, 128, 64, 1);
  OLED_ShowString(4,53,1,"RGB_INDY"); 
  oled.display();
  frame_s = (frame_s + 1) % FRAME_COUNT_48;
  delay(40);
}

bool is_R=false,is_G=false,is_B=false;
int RGB_INDY_index=1;
void RGB_indy_second_display(){
  oled.clearDisplay();
  oled.drawRect(0, 0, 128, 64, 1);
  oled.drawRect(2, 2, 124, 60, 1);

  if(!is_R&&!is_G&&!is_B){
  //button_choise
    if(ScanKey(B_R_)==1)
      RGB_INDY_index++;
    if(ScanKey(B_L_)==1)
      RGB_INDY_index--;

    switch (RGB_INDY_index)
    {
    case 0:
      RGB_INDY_index=4;
      break;
    case 1:
      oled.drawRect(4,10,125,10,1);
      if(ScanKey(B_M_)==1){
        delay(100);
        rgb_isopen=!rgb_isopen;
      }
      break;
    case 2:
      oled.drawRect(4,20,125,10,1);
      if(ScanKey(B_M_)==1)
        is_R=true;
      break;
    case 3:
      oled.drawRect(4,30,125,10,1);
      if(ScanKey(B_M_)==1)
        is_G=true;
      break;
    case 4:
      oled.drawRect(4,40,125,10,1);
      if(ScanKey(B_M_)==1)
        is_B=true;
      break;
    case 5:
      rgb_index=1;
      break;
    }
    if(rgb_isopen)
      OLED_ShowString(5,12,1,"ON");
    else
      OLED_ShowString(5,12,1,"OFF");

    OLED_ShowString(5,22,1,"R");
    OLED_ShowNum(20,22,1,rgb_R);
    OLED_ShowString(5,32,1,"G");
    OLED_ShowNum(20,32,1,rgb_G);
    OLED_ShowString(5,42,1,"B");
    OLED_ShowNum(20,42,1,rgb_B);
  }
  if(is_R){
    delay(100);
    OLED_ShowString(20,15,4,"R");

    OLED_ShowString(70,10,2,"G");
    OLED_ShowString(85,10,2,"B");
    if(ScanKey(B_M_)==1){
        is_R=false;
      }
      if(ScanKey(B_R_)==1){
      rgb_R+=10;
      if(rgb_R>=256)
        rgb_R=10;
      if(rgb_R<=0)
        rgb_R=255;
      }
      if(ScanKey(B_L_)==1){
      rgb_R-=10;
      if(rgb_R>=256)
        rgb_R=10;
      if(rgb_R<=0)
        rgb_R=255;
      }
     OLED_ShowNum(65,30,3,rgb_R);
     
  }
  if(is_G){
    delay(100);
    OLED_ShowString(20,15,4,"G");
    
    OLED_ShowString(70,10,2,"R");
    OLED_ShowString(85,10,2,"B");
    if(ScanKey(B_M_)==1){
        is_G=false;
      }
      if(ScanKey(B_R_)==1){
      rgb_G+=10;
      if(rgb_G>=256)
        rgb_G=10;
      if(rgb_G<=0)
        rgb_G=255;
      }
      if(ScanKey(B_L_)==1){
      rgb_G-=10;
      if(rgb_G>=256)
        rgb_G=10;
      if(rgb_G<=0)
        rgb_G=255;
      }
     OLED_ShowNum(60,30,3,rgb_G);
  }
  if(is_B){
    delay(100);
    OLED_ShowString(20,15,4,"B");
    
    OLED_ShowString(70,10,2,"R");
    OLED_ShowString(85,10,2,"G");
    if(ScanKey(B_M_)==1){
        is_B=false;
      }
      if(ScanKey(B_R_)==1){
      rgb_B+=10;
      if(rgb_B>=256)
        rgb_B=10;
      if(rgb_B<=0)
        rgb_B=255;
      }
      if(ScanKey(B_L_)==1){
      rgb_B-=10;
      if(rgb_B>=256)
        rgb_B=10;
      if(rgb_B<=0)
        rgb_B=255;
      }
     OLED_ShowNum(60,30,3,rgb_B);
  }
  oled.display();
  delay(40);
}

/*-----------------------------SET&INFO-------------------------------*/

void set_info_display(){
  oled.clearDisplay();
  oled.drawBitmap(42, 5, f_set[frame_s], FRAME_WIDTH_48, FRAME_HEIGHT_48, 1);
  oled.drawRect(0, 0, 128, 64, 1);
  OLED_ShowString(4,53,1,"SET&INFO"); 
  oled.display();
  frame_s = (frame_s + 1) % FRAME_COUNT_48;
  delay(40);
}

/*-----------------------------Wifi_web---------------------------*/

bool C_new_wifi=false;
void Wifi_web_display(){
  oled.clearDisplay();
  oled.drawBitmap(42, 5, f_wifi[frame_s], FRAME_WIDTH_48, FRAME_HEIGHT_48, 1);
  oled.drawRect(0, 0, 128, 64, 1);
  OLED_ShowString(4,53,1,"CONNECT_WIFI"); 
  oled.display();
  frame_s = (frame_s + 1) % FRAME_COUNT_48;
  C_new_wifi=false;
  delay(40);
}

int T__=0;

int wifi_index=1;
void Wifi_web_second_display(){
  oled.clearDisplay();
  oled.drawRect(0, 0, 128, 64, 1);
  oled.drawRect(2, 2, 124, 60, 1);
  if(!C_new_wifi){
    if(ScanKey(B_R_)==1)
      wifi_index++;
    if(ScanKey(B_L_)==1)
      wifi_index--;
    switch (wifi_index)
    {
    case 0:
      wifi_index=2;
      break;
    case 1:
      oled.drawRect(4,20,125,10,1);
      if(ScanKey(B_M_)==1){
         oled.clearDisplay();
         OLED_ShowString(5,22,1,"Wsit 10s");
         oled.display();
         Wifi_connect();
        if (WiFi.status() == WL_CONNECTED) {
            now_weather_quest(); 
            pre_weather_quest();
            time_quest();
        }
      }
      break;
    case 2:
    oled.drawRect(4,30,125,10,1);
      if(ScanKey(B_M_)==1)
        C_new_wifi=true;
      break;
    case 3:
      wifi_index=1;
      break;
    }

    OLED_ShowString(5,22,1,"connect:");
    String wifiName= get_String(EEPROM.read(SISSD_ADDR),SISSD_ADDR+1);
    OLED_ShowString(60,22,1,wifiName);
    OLED_ShowString(5,32,1,"connect_NEW_wifi");
  }
 
  if(C_new_wifi){
    oled.clearDisplay();
    oled.drawRect(0, 0, 128, 64, 1);
    oled.drawRect(2, 2, 124, 60, 1);
    OLED_ShowString(5,12,1,"connect and login");
    OLED_ShowString(5,22,1,"192.168.4.1");
    OLED_ShowString(5,32,1,"name:");
    OLED_ShowString(35,32,1,STASSID);
    OLED_ShowString(5,42,1,"pwd:");
    OLED_ShowString(35,42,1,STAPSK);
    oled.display();
    htmlConfig();
    if (WiFi.status() == WL_CONNECTED) {
      now_weather_quest(); 
      pre_weather_quest();
      time_quest();
      C_new_wifi=false;
    }
    
  }
  oled.display();
  delay(40);
}

/*-----------------------------mpu6050------------------------------------*/
void MPU6050_display(){
  oled.clearDisplay();
  ReadMPU6050();
  /*// 串口绘图仪 可视化线加速度曲线
  // Serial.print("Acc_x:");
  // Serial.print(mpu6050_data.Acc_X);
  // Serial.print(",");
  // Serial.print("Acc_Y:");
  // Serial.print(mpu6050_data.Acc_Y);
  // Serial.print(",");
  // Serial.print("Acc_Z:");
  // Serial.println(mpu6050_data.Acc_Z);

  // 串口绘图仪 可视化角速度曲线
  Serial.print("Angle_velocity_R:");
  Serial.println(mpu6050_data.Angle_Velocity_R);
  Serial.print(",");
  Serial.print("Angle_velocity_P:");
  Serial.print(mpu6050_data.Angle_Velocity_P);
  Serial.print(",");
  Serial.print("Angle_velocity_Y:");
  Serial.println(mpu6050_data.Angle_Velocity_Y);
  */
  OLED_ShowNum(10,10,1,mpu6050_data.Acc_X);
  OLED_ShowNum(10,20,1,mpu6050_data.Acc_Y);
  OLED_ShowNum(10,30,1,mpu6050_data.Acc_Z);
  oled.display();
  delay(40);
}