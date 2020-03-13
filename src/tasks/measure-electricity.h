#ifndef TASK_MEASURE_ELECTRICITY
#define TASK_MEASURE_ELECTRICITY

#include <Arduino.h>
#include "EmonLib.h"

#include "../config/config.dist.h"
#include "../config/enums.h"
#include "mqtt-aws.h"

extern MQTTClient mqtt;
extern DisplayValues gDisplayValues;
extern EnergyMonitor emon1;
extern EnergyMonitor emon2;
extern EnergyMonitor emon3;
extern EnergyMonitor emon4;

void measureElectricity(void * parameter)
{
    for(;;){
      serial_println("[ENERGY] Measuring...");
      long start = millis();

      emon1.calcVI(20,2000);
      double watts1 = emon1.realPower;
      double amps2 = emon2.calcIrms(1480);
      double watts2 = amps2 * VOLTAGE_2;
      double amps3 = emon3.calcIrms(1480);
      double watts3 = amps3 * VOLTAGE_3;
      double amps4 = emon4.calcIrms(1480);
      double watts4 = amps4 * VOLTAGE_1;

      gDisplayValues.amps1 = emon1.Vrms;
      gDisplayValues.watt1 = watts1;
      gDisplayValues.amps2 = amps2;
      gDisplayValues.watt2 = watts2;
      gDisplayValues.amps3 = amps3;
      gDisplayValues.watt3 = watts3;
      gDisplayValues.wattDiff = watts2 - watts1;

      // MQTT Sending
      if(!WiFi.isConnected() || !mqtt.connected()){
            serial_println("[MQTT] MQTT: no connection. Discarding data..");
      }
      else {
        serial_print("[MQTT] MQTT publish 1: ");
        serial_println(String(watts1).c_str());
        mqtt.publish(MQTT_TOPIC1, String(watts1).c_str());

        serial_print("[MQTT] MQTT publish 2: ");
        serial_println(String(watts2).c_str());
        mqtt.publish(MQTT_TOPIC2, String(watts2).c_str());

        serial_print("[MQTT] MQTT publish 3: ");
        serial_println(String(watts3).c_str());
        mqtt.publish(MQTT_TOPIC3, String(watts3).c_str());

        serial_print("[MQTT] MQTT publish 4: ");
        serial_println(String(watts4).c_str());
        mqtt.publish(MQTT_TOPIC4, String(watts4).c_str());
      }

      long end = millis();

      // Schedule the task to run again in 1 second (while
      // taking into account how long measurement took)
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }    
}

#endif
