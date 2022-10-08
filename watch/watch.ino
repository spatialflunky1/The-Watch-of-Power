#include <SPI.h>
// Display headers
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

// Clock headers
#include "RTClib.h"

// Initialize clock type
RTC_DS3231 rtc;

int year;
int month;
int day;
int hour;
int minute;
int second;
bool pm;
int count = 0;

void Clear_Screen() {
  LCD_Clear(0xffff);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
}

void setup() {
  Serial.begin(9600);

  // if (! rtc.begin()) {
  //   Serial.println("Couldn't find RTC");
  //   Serial.flush();
  //   while (1) delay(10);
  // }

  // if (rtc.lostPower()) {
  //   // Going to do some time adjustment madness with this code in the future
  //   Serial.println("RTC lost power, let's set the time!");
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //   // This line sets the RTC with an explicit date & time, for example to set
  //   // January 21, 2014 at 3am you would call:
  //   // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  // }

  // Initialize buttons
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);

  // Initialize display
  Config_Init();
  LCD_Init();
  Clear_Screen();

  // Print initial count 
  Paint_DrawNum((LCD_WIDTH/2)-(8*6),LCD_HEIGHT/2,count,&Font20,WHITE,BLACK);
}

void loop() {
  int buttons[3] = {digitalRead(2), digitalRead(3), digitalRead(4)};
  Serial.print(buttons[0]);
  Serial.print(buttons[2]);
  Serial.print("");
  // Gets the current time and date from the rtc
  // DateTime now = rtc.now();
  // year = now.year();
  // month = now.month();
  // day = now.day();
  // hour = now.hour();
  // minute = now.minute();
  // second = now.second();
  // Serial.println("second:");
  // Serial.println(second);
  
  // // Checks if in the morning or afternoon
  // if (hour > 12) pm=true;
  // else pm=false;

  // if (pm) hour-=12;
  
  // PAINT_TIME current = {year,month,day,hour,minute,second};
  // Paint_DrawTime((LCD_WIDTH/2)-(8*6),LCD_HEIGHT/2,&current,&Font20,WHITE,BLACK);
  // delay(750);
  // Paint_DrawTime((LCD_WIDTH/2)-(8*6),LCD_HEIGHT/2,&current,&Font20,WHITE,WHITE);

  if (buttons[0] == HIGH) {
    Paint_DrawNum((LCD_WIDTH/2)-(8*6),LCD_HEIGHT/2,count,&Font20,WHITE,WHITE);
    count -=1;
    Paint_DrawNum((LCD_WIDTH/2)-(8*6),LCD_HEIGHT/2,count,&Font20,WHITE,BLACK);
  }
  if (buttons[2] == HIGH) {
    Paint_DrawNum((LCD_WIDTH/2)-(8*6),LCD_HEIGHT/2,count,&Font20,WHITE,WHITE);
    count +=1;
    Paint_DrawNum((LCD_WIDTH/2)-(8*6),LCD_HEIGHT/2,count,&Font20,WHITE,BLACK);
  }
}
