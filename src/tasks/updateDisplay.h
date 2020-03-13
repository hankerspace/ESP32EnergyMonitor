#ifndef TASK_UPDATE_DISPLAY
#define TASK_UPDATE_DISPLAY

#include <Arduino.h>
#include "heltec.h"
#include "functions/drawFunctions.h"
#include "../config/config.dist.h"

//extern Adafruit_SSD1306 display;
extern DisplayValues gDisplayValues;

/**
 * Metafunction that takes care of drawing all the different
 * parts of the display (or not if it's turned off).
 */
void updateDisplay(void * parameter){
  for (;;){
    serial_println(F("[LCD] Updating..."));
    Heltec.display->clear();

    if(gDisplayValues.currentState == CONNECTING_WIFI || 
        gDisplayValues.currentState == CONNECTING_AWS){
      drawBootscreen();
    }
    
    if(gDisplayValues.currentState == UP){
      drawTime();
      drawSignalStrength();
      drawAmpsWatts();
    }

    Heltec.display->display();

    // Sleep for 2 seconds, then update display again!
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

#endif
