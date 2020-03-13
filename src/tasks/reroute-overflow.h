#ifndef TASK_REROUTE_OVERFLOW
#define TASK_REROUTE_OVERFLOW

#include <Arduino.h>
#include "EmonLib.h"
#include "WiFi.h"
#include "heltec.h"
#include <SPI.h>
#include "../config/enums.h"
#include "../config/config.dist.h"

extern EnergyMonitor emon1;

uint ledPower = 0;
/**
 * TASK: get the overflow power and reroute it 
 */
void rerouteOverflow(void * parameter){
  int count = 0;
  for(;;){


    float overflow = emon1.realPower > 0 ? 0 : -emon1.realPower;
    if(overflow > 0) {
      ledPower++;
    }
    else {
      ledPower--;
    }

    // Log every seconds
    if(count % 10 == 0) {
      serial_print("[ROUTER] Rerouting ");
      serial_print(overflow);
      serial_print("W overflow... Current led power: ");
      serial_println(ledPower);
      count = 0;
    }
    
    ledcWrite(LEDC_CHANNEL_0, constrain(ledPower, 0, 255));
    //analogWrite(ADC_OUTPUT_LED, constrain(ledPower, 0.0f, 5.0f));

    count++;
    // Sleep for 100ms
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

#endif