#ifndef TASK_REROUTE_OVERFLOW
#define TASK_REROUTE_OVERFLOW

#include <Arduino.h>
#include "EmonLib.h"
#include "WiFi.h"
#include "../config/enums.h"
#include "../config/config.dist.h"

extern EnergyMonitor emon1;

float ledPower = 0;
/**
 * TASK: get the overflow power and reroute it 
 */
void rerouteOverflow(void * parameter){
  int count = 0;
  for(;;){


    float overflow = emon1.realPower > 0 ? 0 : -emon1.realPower;
    if(overflow > 0) {
      ledPower+=0,01f;
    }
    else {
      ledPower-=0,01f;
    }

    // Log every seconds
    if(count % 10 == 0) {
      serial_println("[ROUTER] Rerouting %fW overflow... Current led power: %f", overflow, ledPower);
      count = 0;
    }

    analogWrite(ADC_OUTPUT_LED, ledPower);

    count++;
    // Sleep for 100ms
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

#endif