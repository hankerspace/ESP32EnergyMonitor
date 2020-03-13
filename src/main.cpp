#include <Arduino.h>
#include "heltec.h"
#include <SPI.h>
#include <Wire.h>
#include "EmonLib.h"
#include <driver/adc.h>

#include "config/config.dist.h"
#include "config/enums.h"

#include "tasks/fetch-time-from-ntp.h"
#include "tasks/mqtt-aws.h"
#include "tasks/wifi-connection.h"
#include "tasks/reroute-overflow.h"
#include "tasks/wifi-update-signalstrength.h"
#include "tasks/measure-electricity.h"
#include "tasks/updateDisplay.h"


DisplayValues gDisplayValues;
EnergyMonitor emon1;
EnergyMonitor emon2;
EnergyMonitor emon3;
EnergyMonitor emon4;

// Place to store local measurements before sending them off to AWS
unsigned short measurements1[LOCAL_MEASUREMENTS];
unsigned short measurements2[LOCAL_MEASUREMENTS];
unsigned short measurements3[LOCAL_MEASUREMENTS];
unsigned char measureIndex = 0;

void setup()
{
  Serial.begin(115200);

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, false /*Serial Disable*/);

  // Setup the ADC
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_1, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_2, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
  analogReadResolution(ADC_BITS);
  pinMode(ADC_INPUT1, INPUT);
  pinMode(ADC_INPUT2, INPUT);
  pinMode(ADC_INPUT3, INPUT);
  pinMode(ADC_INPUT4, INPUT);
  pinMode(V_INPUT, INPUT);
  pinMode(ADC_OUTPUT_LED, OUTPUT);

  emon1.voltage(V_INPUT, 180, 1.7);             // Current: input pin, calibration.
  emon1.current(ADC_INPUT1, 28);             // Current: input pin, calibration.
  emon2.current(ADC_INPUT2, 14);             // Current: input pin, calibration.
  emon3.current(ADC_INPUT3, 14);             // Current: input pin, calibration.
  emon4.current(ADC_INPUT4, 14);             // Current: input pin, calibration.

  // ----------------------------------------------------------------
  // TASK: Connect to WiFi & keep the connection alive.
  // ----------------------------------------------------------------
  xTaskCreatePinnedToCore(
    keepWiFiAlive,
    "keepWiFiAlive",  // Task name
    5000,            // Stack size (bytes)
    NULL,             // Parameter
    1,                // Task priority
    NULL,             // Task handle
    ARDUINO_RUNNING_CORE
  );

  // ----------------------------------------------------------------
  // TASK: Connect to AWS & keep the connection alive.
  // ----------------------------------------------------------------
  xTaskCreate(
    keepAWSConnectionAlive,
    "MQTT-AWS",      // Task name
    5000,            // Stack size (bytes)
    NULL,             // Parameter
    5,                // Task priority
    NULL              // Task handle
  );

  // ----------------------------------------------------------------
  // TASK: Update the display every second
  //       This is pinned to the same core as Arduino
  //       because it would otherwise corrupt the OLED
  // ----------------------------------------------------------------
  xTaskCreatePinnedToCore(
    updateDisplay,
    "UpdateDisplay",  // Task name
    10000,            // Stack size (bytes)
    NULL,             // Parameter
    3,                // Task priority
    NULL,             // Task handle
    ARDUINO_RUNNING_CORE
  );

  // ----------------------------------------------------------------
  // Task: measure electricity consumption ;)
  // ----------------------------------------------------------------
  xTaskCreate(
    measureElectricity,
    "Measure electricity",  // Task name
    10000,                  // Stack size (bytes)
    NULL,                   // Parameter
    4,                      // Task priority
    NULL                    // Task handle
  );

  // ----------------------------------------------------------------
  // Task: reroute overflow
  // ----------------------------------------------------------------
  xTaskCreate(
    rerouteOverflow,
    "Reroute overflow",  // Task name
    1000,                  // Stack size (bytes)
    NULL,                   // Parameter
    7,                      // Task priority
    NULL                    // Task handle
  );

  // ----------------------------------------------------------------
  // TASK: update time from NTP server.
  // ----------------------------------------------------------------
  #if NTP_TIME_SYNC_ENABLED == true
    xTaskCreate(
      fetchTimeFromNTP,
      "Update NTP time",
      5000,            // Stack size (bytes)
      NULL,             // Parameter
      1,                // Task priority
      NULL              // Task handle
    );
  #endif

  // ----------------------------------------------------------------
  // TASK: update WiFi signal strength
  // ----------------------------------------------------------------
  xTaskCreate(
    updateWiFiSignalStrength,
    "Update WiFi strength",
    1000,             // Stack size (bytes)
    NULL,             // Parameter
    2,                // Task priority
    NULL              // Task handle
  );

}

void loop()
{
    vTaskDelay(10000 / portTICK_PERIOD_MS);
}