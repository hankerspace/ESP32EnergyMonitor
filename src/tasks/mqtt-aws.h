#ifndef TASK_MQTT_AWS
#define TASK_MQTT_AWS

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <pgmspace.h>
#include <WiFi.h>
#include "../config/config.dist.h"

#define MAX_MSG_SIZE_BYTES 900

MQTTClient mqtt = MQTTClient(MAX_MSG_SIZE_BYTES);
WiFiClient net;

void keepAWSConnectionAlive(void * parameter){
    for(;;){
        if(mqtt.connected()){
            mqtt.loop();
            vTaskDelay(500 / portTICK_PERIOD_MS);
            continue;
        }

        if(!WiFi.isConnected()){
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        serial_println(F("[MQTT] Connecting to MQTT..."));
        mqtt.begin(MQTT_ENDPOINT, net);

        long startAttemptTime = millis();
    
        while (!mqtt.connect(DEVICE_NAME) && (millis() - startAttemptTime < MQTT_CONNECT_TIMEOUT))
        {
            vTaskDelay(MQTT_CONNECT_DELAY);
        }

        if(!mqtt.connected()){
            lwmqtt_err_t lastError = mqtt.lastError();
            lwmqtt_return_code_t returnCode = mqtt.returnCode();
            serial_println("[MQTT] Last error:" + String(lastError) + " return code:" +String(returnCode));
            serial_println(F("[MQTT] MQTT connection timeout. Retry in 30s."));
            vTaskDelay(30000 / portTICK_PERIOD_MS);
        }

        serial_println(F("[MQTT] MQTT Connected!"));
    }
}

/**
 * TASK: Upload measurements to AWS. This only works when there are enough
 * local measurements. It's called by the measurement function.
 */
// void uploadMeasurementsToMQTT(void * parameter){
//     if(!WiFi.isConnected() || !mqtt.connected()){
//         serial_println("[MQTT] MQTT: no connection. Discarding data..");
//         vTaskDelete(NULL);
//     }

//     char msg[AWS_MAX_MSG_SIZE_BYTES];
//     strcpy(msg, "{\"readings1\":[");

//     for (short i = 0; i < LOCAL_MEASUREMENTS-1; i++){
//         strcat(msg, String(measurements1[i]).c_str());
//         strcat(msg, ",");
//     }

//     strcat(msg, String(measurements1[LOCAL_MEASUREMENTS-1]).c_str());
//     strcat(msg, "]}");

//     strcpy(msg, "{\"readings2\":[");

//     for (short i = 0; i < LOCAL_MEASUREMENTS-1; i++){
//         strcat(msg, String(measurements2[i]).c_str());
//         strcat(msg, ",");
//     }

//     strcat(msg, String(measurements2[LOCAL_MEASUREMENTS-1]).c_str());
//     strcat(msg, "]}");

//     strcpy(msg, "{\"readings3\":[");

//     for (short i = 0; i < LOCAL_MEASUREMENTS-1; i++){
//         strcat(msg, String(measurements3[i]).c_str());
//         strcat(msg, ",");
//     }

//     strcat(msg, String(measurements3[LOCAL_MEASUREMENTS-1]).c_str());
//     strcat(msg, "]}");
        
//     serial_print("[MQTT] MQTT publish: ");
//     serial_println(msg);
//     mqtt.publish(MQTT_TOPIC, msg);

//     // Task is done!
//     vTaskDelete(NULL);
// }
#endif
