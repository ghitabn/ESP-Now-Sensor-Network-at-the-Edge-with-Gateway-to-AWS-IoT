#define NODE_ID    3   // unique ID for this node

// IS_SENSOR or IS_REPEATER or IS_GATEWAY
//#define IS_SENSOR
//#define IS_REPEATER
#define IS_GATEWAY

#ifdef IS_SENSOR
// sensor type
//#define DUMMY_SENSOR
#define DHT_SENSOR
#endif

// use ESPNow protocol
#define USE_ESPNOW

// debugging via serial console
#define DEBUG_ON

// gateway MAC address

// esp8266: DC:4F:22:18:46:1C
//uint8_t gatewayAddress[] = {0xDC, 0x4F, 0x22, 0x18, 0x46, 0x1C};

// esp32: EC:94:CB:6C:B0:80
uint8_t gatewayAddress[] = {0xEC, 0x94, 0xCB, 0x6C, 0xB0, 0x80};

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <espnow.h>
#endif
#ifdef ESP32
  #include <esp_now.h>
  #include <WiFi.h>
  #include <esp_wifi.h>
#endif