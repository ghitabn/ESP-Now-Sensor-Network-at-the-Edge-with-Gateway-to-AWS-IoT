// structure to send/receive data
typedef struct dataReading {
  uint16_t id;
  float data;
  uint8_t type;
} dataReading;

// create a data reading object
dataReading currentDataReading;

#ifdef USE_ESPNOW
  #include <node_espnow.h>
#endif

// init node
void beginNode()
{
#ifdef DEBUG_ON
  Serial.begin(115200);
#endif

#ifdef IS_GATEWAY
  #ifdef ESP8266
  Serial1.begin(115200);
  #endif
  #ifdef ESP32
  Serial2.begin(115200);
  #endif
#endif

// init ESP-NOW for ESP8266 or ESP32
#ifdef USE_ESPNOW
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
#ifdef ESP8266
  if (esp_now_init() != 0)
  {
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  // register peers
  esp_now_add_peer(gatewayAddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0);
#endif
#ifdef ESP32
  if (esp_now_init() != ESP_OK)
  {
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo;
  peerInfo.ifidx = WIFI_IF_STA;
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
 
  memcpy(peerInfo.peer_addr, gatewayAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    return;
  }
#endif
#endif // USE_ESPNOW
}

// function to send data
bool sendNodeData()
{
#ifdef USE_ESPNOW
  esp_now_send(gatewayAddress, (uint8_t *) &currentDataReading, sizeof(currentDataReading));
  return true;
#endif
}