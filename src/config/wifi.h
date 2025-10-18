#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <Arduino.h>

// WiFi credentials
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

// Function declarations
void initWiFi();
bool isWiFiConnected();
String getIPAddress();

#endif

