#ifndef CONFIG
#define CONFIG

/**
 * Set this to false to disable Serial logging
 */
#define DEBUG true

/**
 * The name of this device (as defined in the AWS IOT console).
 * Also used to set the hostname on the network
 */
#define DEVICE_NAME "ESP32Wattmeter"

// 1 : to solar
// 2 : from solar
// 3 : ?

/**
 * ADC input pin that is used to read out the CT sensor
 */
#define ADC_INPUT1 36
#define ADC_INPUT2 37
#define ADC_INPUT3 38
#define ADC_INPUT4 34

#define V_INPUT 39

#define ADC_OUTPUT_LED 25

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0
// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13
// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

/**
 * The voltage of your home, used to calculate the wattage.
 * Try setting this as accurately as possible.
 */
#define VOLTAGE_1 235.0
#define VOLTAGE_2 230.0
#define VOLTAGE_3 235.0

/**
 * WiFi credentials
 */
#define WIFI_NETWORK "CUIR"
#define WIFI_PASSWORD "****"

/**
 * Timeout for the WiFi connection. When this is reached,
 * the ESP goes into deep sleep for 30seconds to try and
 * recover.
 */
#define WIFI_TIMEOUT 20000 // 20 seconds

/**
 * How long should we wait after a failed WiFi connection
 * before trying to set one up again.
 */
#define WIFI_RECOVER_TIME_MS 20000 // 20 seconds

/**
 * Dimensions of the OLED display attached to the ESP
 */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

/**
 * Force Emonlib to assume a 3.3V supply to the CT sensor
 */
#define emonTxV3 1


/**
 * Local measurements
 */
#define LOCAL_MEASUREMENTS 30


/**
 * The MQTT endpoint of the service we should connect to and receive messages
 * from.
 */
#define AWS_ENABLED true
#define MQTT_ENDPOINT "192.168.86.40"
#define MQTT_TOPIC1 "energy/watts1"
#define MQTT_TOPIC2 "energy/watts2"
#define MQTT_TOPIC3 "energy/watts3"
#define MQTT_TOPIC4 "energy/watts4"

#define MQTT_CONNECT_DELAY 200
#define MQTT_CONNECT_TIMEOUT 20000 // 20 seconds


/**
 * Syncing time with an NTP server
 */
#define NTP_TIME_SYNC_ENABLED true
#define NTP_SERVER "pool.ntp.org"
#define NTP_OFFSET_SECONDS 3600
#define NTP_UPDATE_INTERVAL_MS 60000


// Check which core Arduino is running on. This is done because updating the 
// display only works from the Arduino core.
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#endif
