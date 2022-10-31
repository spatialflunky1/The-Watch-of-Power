#include <SPI.h>
// Display headers
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// Clock headers
#include "RTClib.h"

// Define pins
#define TFT_DC 7
#define TFT_CS 10
#define TFT_RST 8
#define leftButton 4
#define middleButton 5
#define rightButton 6

// Initialize clock type
RTC_DS3231 rtc;

int hour;
int minute;
int second;
bool pm;
int lastSecond=0;
int lastMinute=0;
int lastHour=0;
char* screenState;
char* settingState;
int y=100;
int selectedApp=0;
int buttonPressed=0;
char *apps[] = {"Clock","Settings"};
int numApps = sizeof(apps)/sizeof(apps[0])-1;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

String getMinOrSec(int mos){
  if (mos < 10) return ("0"+String(mos));
  else return (String(mos));
}

void Update_Time() {
  tft.setCursor(80, y);
  tft.setTextColor(ILI9341_BLACK);
  tft.println(String(lastHour)+":"+getMinOrSec(lastMinute)+":"+getMinOrSec(lastSecond));
  tft.setCursor(80, y);
  tft.setTextColor(ILI9341_GREEN);
  tft.println(String(hour)+":"+getMinOrSec(minute)+":"+getMinOrSec(second));
  lastSecond = second;
  lastMinute = minute;
  lastHour = hour;
}

void setup() {
  Serial.begin(9600);

  tft.begin();
  tft.setRotation(1);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(4);
  tft.fillScreen(ILI9341_BLACK);

  // Initialize buttons
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  if (! rtc.begin()) {
   Serial.println("Couldn't find RTC");
   Serial.flush();
   while (1) delay(10);
  }

  if (rtc.lostPower()) {
    // Going to do some time adjustment madness with this code in the future
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  screenState=malloc(100);
  screenState=strcpy(screenState, "Home Screen");
  tft.setCursor(80, y);
  tft.println(apps[selectedApp]);
}

void set_screenState() {
  if (buttonPressed == 0) {
    if (strcmp(screenState, "Home Screen")==0) {
      if (digitalRead(leftButton)==HIGH) {
        if (selectedApp > 0) selectedApp--;
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(80, y);
        tft.println(apps[selectedApp]);
        buttonPressed=1;
      }
      if (digitalRead(middleButton)==HIGH) {
        screenState=strcpy(screenState, apps[selectedApp]);
        buttonPressed=1;
      }
      if (digitalRead(rightButton)==HIGH) {
        if (selectedApp < numApps) selectedApp++;
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(80, y);
        tft.println(apps[selectedApp]);
        buttonPressed=1;
      }
    }

    if (strcmp(screenState, "Clock")==0) {
      tft.fillScreen(ILI9341_BLACK);
      while (true) {
        if (digitalRead(middleButton)==HIGH) {
          screenState=strcpy(screenState, "Home Screen");
          tft.fillScreen(ILI9341_BLACK);
          break;
        }
        clock();
      }
    }
  }
}

void clock() {
  // Gets the current time and date from the rtc
  DateTime now = rtc.now();
  hour = now.hour();
  minute = now.minute();
  second = now.second();

  // Checks if in the morning or afternoon
  if (hour > 12) pm=true;
  else pm=false;

  if (pm) hour-=12;

  if (second != lastSecond) Update_Time();
}

void loop() {
  set_screenState();
  if (buttonPressed==1 && digitalRead(leftButton)==LOW && digitalRead(middleButton)==LOW && digitalRead(rightButton)==LOW) buttonPressed=0;
}
