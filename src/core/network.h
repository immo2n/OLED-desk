#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// WiFi functions (prefixed with network to avoid conflicts)
void networkInitWiFi(const char* ssid, const char* password);
bool networkIsWiFiConnected();
String networkGetIPAddress();

// HTTP functions
String httpGet(const String& url, bool useHttps = false);

#endif

