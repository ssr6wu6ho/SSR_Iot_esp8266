#include "All_display_.hpp"
#include "RGB_.hpp"

struct KEY_TABLE
{
  int index;
  int next;
  int last;
  int Return_;
  int Enter_;
  void (*operation)(void);
};

KEY_TABLE Page_table[20] = {
    {0, 0, 0, 0, 1, (*bide_display)},

    {1, 2, 7, 0, 1, (*time_main_display)},
    {2, 3, 1, 0, 2, (*preweather_display)},
    {3, 4, 2, 0, 8, (*rgb_control_display)},
    {4, 5, 3, 0, 9, (*RGB_indy_set_display)},
    {5, 6, 4, 0, 10, (*Wifi_web_display)},
    {6, 7, 5, 0, 11, (*Mqtt_display)},
    {7, 1, 6, 0, 7, (*All_message_display)},

    {8, 8, 8, 3, 8, (*rgb_second_display)},
    {9, 9, 9, 4, 9, (*RGB_indy_second_display)},
    {10, 10, 10, 5, 10, (*Wifi_web_second_display)},
    {11, 11, 11, 6, 11, (*Mqtt_second_display)}

};
void bide_display();
void time_main_display();
void preweather_display();
void rgb_control_display();
void rgb_second_display();
void RGB_indy_set_display();
void RGB_indy_second_display();
void Wifi_web_display();
void Wifi_web_second_display();
void Mqtt_display();
void Mqtt_second_display();
void All_message_display();
void (*current)();

/*------------------------main-----------------------*/

int display_flag = 0;
void Menu_Choise()
{
  if (ScanKey(B_M_) == 1)
  {
    display_flag = Page_table[display_flag].Enter_;
  }
  if (ScanKey(B_R_) == 1)
  {
    fadeEffect(BLACK, WHITE, 25); // 从黑色渐变到白色，共25步
    display_flag = Page_table[display_flag].next;
  }
  if (ScanKey(B_L_) == 1)
  {
    fadeEffect(BLACK, WHITE, 25); 
    display_flag = Page_table[display_flag].last;
  }
  if (ScanKey(B_L_) == 1 && ScanKey(B_R_) == 1)
  {
    display_flag = Page_table[display_flag].Return_;
  }
  current = Page_table[display_flag].operation;
  (*current)();
}

void setup()
{
  Serial.begin(9600);
  OLED_Init();
  RGB_init();

  EEPROM.begin(1024);
}

// main loop
void loop()
{
  Menu_Choise();
  if (!rgb_isopen)
  {
    _RGB_.clear();
    _RGB_.show();
  }
  else
    RGB_conduct();
}

// 渐变背景过渡效果
void fadeEffect(int startColor, int endColor, int steps)
{
  for (int i = 0; i < steps; i++)
  {
    int color = (startColor + (endColor - startColor) * i / steps);
    oled.clearDisplay();
    oled.fillRect(0, 0, 128, 64, color); // 假设oled是0.96寸OLED的实例
    oled.display();
    delay(10);
  }
}
