#include "wifi.h"
#include "../core/network.h"

// WiFi credentials
const char* WIFI_SSID = "Moon-iOT";
const char* WIFI_PASSWORD = "moon_iot_182838";

void initWiFi() {
    // Call the core network init function with our credentials
    networkInitWiFi(WIFI_SSID, WIFI_PASSWORD);
}

bool isWiFiConnected() {
    return networkIsWiFiConnected();
}

String getIPAddress() {
    return networkGetIPAddress();
}

