#include <WiFi.h>
#include <esp_wifi.h>

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

typedef struct dataReading {
  uint16_t id;
  float data;
  uint8_t type;
} dataReading;

// create a data reading object
dataReading currentDataReading;

uint8_t incomingData[sizeof(struct dataReading)];
size_t incomingDataLength;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

#define AWS_IOT_PUBLISH_TOPIC "esp32/gateway"

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to WiFi: " + String(WIFI_SSID));

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["id"] = currentDataReading.id;
  doc["data"] = (String)currentDataReading.data;
  doc["type"] = currentDataReading.type;
  char jsonBuffer[512];
  // print to client
  serializeJson(doc, jsonBuffer);
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  connectAWS();
}

void loop() {
  if (Serial2.available()) {
    incomingDataLength = Serial2.readBytesUntil('\n', incomingData, sizeof(currentDataReading));
    if (incomingDataLength == sizeof(currentDataReading)) {
      memcpy(&currentDataReading, incomingData, sizeof(incomingData));
      Serial.println("rx (serial): " + (String)currentDataReading.id + "," + (String)currentDataReading.type + "," + (String)currentDataReading.data);
      publishMessage();
      client.loop();
    }
  }
  delay(100);
}