#include "network.h"
#include "../config/display.h"
#include "text.h"

void networkInitWiFi(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.println();
    Serial.print("Connecting to WiFi");
    text("Connecting to WiFi...", 0, 0, 1);
    
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 30) {
        delay(500);
        Serial.print(".");
        attempt++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("WiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        
        // Show connection success
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("WiFi Connected!");
        display.setCursor(0, 16);
        display.println("IP:");
        display.setCursor(0, 28);
        display.println(WiFi.localIP());
        display.display();
        delay(2000);
    } else {
        Serial.println();
        Serial.println("WiFi connection failed!");
        text("WiFi Failed!", 0, 0, 1);
        delay(2000);
    }
}

bool networkIsWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String networkGetIPAddress() {
    return WiFi.localIP().toString();
}

String httpGet(const String& url, bool useHttps) {
    if (!networkIsWiFiConnected()) {
        Serial.println("WiFi not connected!");
        return "";
    }
    
    HTTPClient http;
    String payload = "";
    
    if (useHttps) {
        WiFiClientSecure secureClient;
        secureClient.setInsecure(); // Skip certificate verification
        
        http.setTimeout(15000);
        http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
        http.setUserAgent("ESP8266");
        http.setReuse(false);
        
        if (http.begin(secureClient, url)) {
            http.addHeader("Connection", "close");
            http.addHeader("Accept", "application/json");
            
            int httpCode = http.GET();
            
            if (httpCode == HTTP_CODE_OK) {
                payload = http.getString();
            } else {
                Serial.printf("HTTP GET failed, code: %d\n", httpCode);
            }
            
            http.end();
        }
    } else {
        WiFiClient client;
        
        http.setTimeout(15000);
        http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
        http.setUserAgent("ESP8266");
        http.setReuse(false);
        
        if (http.begin(client, url)) {
            http.addHeader("Connection", "close");
            http.addHeader("Accept", "application/json");
            
            int httpCode = http.GET();
            
            if (httpCode == HTTP_CODE_OK) {
                payload = http.getString();
            } else {
                Serial.printf("HTTP GET failed, code: %d\n", httpCode);
            }
            
            http.end();
        }
    }
    
    return payload;
}

