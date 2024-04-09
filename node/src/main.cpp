#include <Arduino.h>
#include <node_config.h>
#include <node.h>
#include <datatypes.h>

#ifdef IS_SENSOR
// sensor specific

#ifdef DHT_SENSOR
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#endif // DHT11_SENSOR
#endif // IS_SENSOR

void setup() {
  beginNode();
  #ifdef IS_SENSOR
  // sensor specific initializations
  currentDataReading.id = NODE_ID;
  
  #ifdef DUMMY_SENSOR
  currentDataReading.data = 0;
  currentDataReading.type = DUMMY_T;
  #endif // DUMMY_SENSOR

  #ifdef DHT_SENSOR
  pinMode(DHTPIN,INPUT);
  dht.begin();
  #endif // DHT_SENSOR
  #endif // IS_SENSOR
  }
 
void loop() {
  #ifdef IS_SENSOR
  // sensor specific actions
  #ifdef DUMMY_SENSOR
  currentDataReading.data = 1 - currentDataReading.data;
  sendNodeData();
  #endif // DUMMY_SENSOR

  #ifdef DHT_SENSOR
  currentDataReading.data = dht.readTemperature();
  if (!isnan(currentDataReading.data))
  {
    currentDataReading.type = TEMP_T;
    sendNodeData();
    delay(500);
  }
  currentDataReading.data = dht.readHumidity();
  if (!isnan(currentDataReading.data))
  {
    currentDataReading.type = HUMIDITY_T;
    sendNodeData();
  }
  #endif // DHT_SENSOR
  
  delay(2000);
  #endif // iS_SENSOR
}