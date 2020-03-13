#ifndef DRAW_FUNCTIONS
#define DRAW_FUNCTIONS

 #include <WiFi.h>
#include "heltec.h"
 #include "../config/enums.h"
 #include "../config/config.dist.h"

// extern Adafruit_SSD1306 display;
extern DisplayValues gDisplayValues;
extern unsigned char measureIndex;

void drawTime(){
    Heltec.display->drawString(0, 0, String(gDisplayValues.time));
//   display.setTextSize(1);
//   display.setCursor(0, 0);
//   display.print(gDisplayValues.time);
}

void drawSignalStrength(){
   const byte X = SCREEN_WIDTH - 20;
   const byte X_SPACING = 2;


  // Draw the four base rectangles
  Heltec.display->fillRect(X, 8-2, 1, 2);
  Heltec.display->fillRect(X + X_SPACING, 8-2, 1, 2);
  Heltec.display->fillRect(X + X_SPACING*2, 8-2, 1, 2);
  Heltec.display->fillRect(X + X_SPACING*3, 8-2, 1, 2);

  // Draw bar 2
  if(gDisplayValues.wifi_strength > -70){
      Heltec.display->fillRect(X+X_SPACING, 8-4, 1, 4);
  }

  // Draw bar 3
  if(gDisplayValues.wifi_strength > -60){
      Heltec.display->fillRect(X+X_SPACING*2, 8-6, 1, 6);
  }

  // Draw bar 4
  if(gDisplayValues.wifi_strength >= -50){
      Heltec.display->fillRect(X+X_SPACING*3, 8-8, 1, 8);
  }
}

// /**
//  * The screen that is displayed when the ESP has just booted
//  * and is connecting to WiFi & AWS.
//  */
void drawBootscreen(){
  byte X = 14;
  byte Y = 70;
  byte WIDTH = 6;
  byte MAX_HEIGHT = 35;
  byte HEIGHT_STEP = 10;
  byte X_SPACING = 10;

    Heltec.display->fillRect(X              , Y, WIDTH, MAX_HEIGHT - HEIGHT_STEP*3);
    Heltec.display->fillRect(X + X_SPACING  , Y - HEIGHT_STEP, WIDTH, MAX_HEIGHT - HEIGHT_STEP*2);
    Heltec.display->fillRect(X + X_SPACING*2, Y - HEIGHT_STEP*2, WIDTH, MAX_HEIGHT - HEIGHT_STEP);
    Heltec.display->fillRect(X + X_SPACING*3, Y - HEIGHT_STEP*3, WIDTH, MAX_HEIGHT);

  Heltec.display->drawString(0, Y + MAX_HEIGHT / 2, "Connecting");

  if(gDisplayValues.currentState == CONNECTING_WIFI){
      Heltec.display->drawString(0, Y + MAX_HEIGHT / 2, "Connecting WiFi");
  }

  if(gDisplayValues.currentState == CONNECTING_AWS){
    Heltec.display->drawString(0, Y + MAX_HEIGHT / 2, "Connecting AWS");
  }
}

// /**
//  * Draw the current amps & watts in the middle of the display.
//  */
void drawAmpsWatts(){

  String watts1 = String(gDisplayValues.watt1, 0);
  String amps1 = String(gDisplayValues.amps1, 2);
String watts2 = String(gDisplayValues.watt2, 0);
  String amps2 = String(gDisplayValues.amps2, 2);
  String watts3 = String(gDisplayValues.watt3, 0);
  String amps3 = String(gDisplayValues.amps3, 2);
    String wattsDiff = String(abs(gDisplayValues.wattDiff), 0);
  String ampsDiff = String(abs(gDisplayValues.ampsDiff), 2);

    Heltec.display->drawString(0, 20, "1 => " + amps1 + " A | " + watts1 + " W");
    Heltec.display->drawString(0, 30, "2 => " + amps2 + " A | " + watts2 + " W");
    Heltec.display->drawString(0, 40, "3 => " + amps3 + " A | " + watts3 + " W");
    if(gDisplayValues.ampsDiff > 0) {
      Heltec.display->drawString(0, 50, "Prod => " + ampsDiff + " A | " + wattsDiff + " W");
    }
    else {
      Heltec.display->drawString(0, 50, "Cons => " + ampsDiff + " A | " + wattsDiff + " W");
    }

}

#endif