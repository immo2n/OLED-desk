#include "clock.h"
#include "../../config/display.h"
#include "../../config/wifi.h"
#include "../../core/network.h"
#include <ArduinoJson.h>

// Time variables
int currentHour = 0;
int currentMinute = 0;
int currentSecond = 0;
int currentDay = 1;
int currentMonth = 1;
int currentYear = 0;
bool timeInitialized = false;
unsigned long lastUpdateTime = 0;
unsigned long lastFetchTime = 0;
const unsigned long FETCH_INTERVAL = 3600000; // Update from API every hour (3600000 ms)

// Marquee variables
int marqueeX = 128; // Start from right edge
String marqueeText = ""; // Will be built dynamically
unsigned long lastMarqueeUpdate = 0;
const unsigned long MARQUEE_SPEED = 50; // milliseconds between updates

// Month names
const char* monthNames[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

// WiFi icon bitmap (8x8 pixels)
const unsigned char wifiIcon[] PROGMEM = {
    0b00000000,
    0b00111100,
    0b01000010,
    0b10011001,
    0b00100100,
    0b00011000,
    0b00011000,
    0b00000000
};

void initClock() {
    // Show loading message
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 25);
    display.println("Fetching time...");
    display.display();
    
    // Initial fetch from API (with retry)
    for (int i = 0; i < 3 && !timeInitialized; i++) {
        if (i > 0) {
            Serial.printf("Retry attempt %d...\n", i);
            delay(2000);
        }
        updateClock();
    }
    
    if (!timeInitialized) {
        Serial.println("Failed to fetch time after 3 attempts!");
        display.clearDisplay();
        display.setCursor(10, 25);
        display.println("Time sync failed!");
        display.display();
        delay(2000);
        lastUpdateTime = millis(); // Set it even if failed, to prevent issues
    }
    // Note: lastUpdateTime is now set when we receive API time for accurate seconds sync
}

void updateClock() {
    if (!isWiFiConnected()) {
        Serial.println("WiFi not connected!");
        return;
    }
    
    Serial.println("\n=== Fetching time from BepariSoft API ===");
    
    // Use beparisoft API
    String url = "https://api.beparisoft.com/";
    Serial.print("Fetching from: ");
    Serial.println(url);
    
    String payload = httpGet(url, true); // true = HTTPS
    
    if (payload.length() > 0) {
        Serial.println("✓ Time data received!");
        Serial.print("Payload length: ");
        Serial.println(payload.length());
            
        // Parse JSON
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        
        if (!error) {
            Serial.println("✓ JSON parsed successfully");
            
            // Get datetime string from nested structure: data.origin.datetime
            const char* datetime = doc["data"]["origin"]["datetime"];
            if (datetime == nullptr) {
                Serial.println("ERROR: data.origin.datetime field is null!");
                return;
            }
            
            Serial.print("Datetime string: ");
            Serial.println(datetime);
            
            // Parse datetime
            int year, month, day, hour, minute, second;
            int parsed = sscanf(datetime, "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second);
            
            Serial.printf("Parsed %d fields: year=%d, month=%d, day=%d, hour=%d, min=%d, sec=%d\n", 
                parsed, year, month, day, hour, minute, second);
            
            if (parsed == 6 && year > 2000 && month >= 1 && month <= 12 && day >= 1 && day <= 31) {
                currentYear = year; // Get full year
                currentMonth = month;
                currentDay = day;
                currentHour = hour;
                currentMinute = minute;
                currentSecond = second;
                
                timeInitialized = true;
                lastFetchTime = millis();
                lastUpdateTime = millis(); // Sync the update timer with API time
                
                Serial.println("✓✓✓ TIME SUCCESSFULLY SET ✓✓✓");
                Serial.printf("Time: %02d:%02d:%02d\n", currentHour, currentMinute, currentSecond);
                Serial.printf("Date: %s %d, %d\n", 
                    monthNames[currentMonth - 1], currentDay, currentYear);
            } else {
                Serial.println("ERROR: Failed to parse datetime fields or invalid values!");
                Serial.printf("Validation: year>2000=%d, month 1-12=%d, day 1-31=%d\n",
                    year > 2000, month >= 1 && month <= 12, day >= 1 && day <= 31);
            }
        } else {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
        }
    } else {
        Serial.println("ERROR: Failed to fetch data from API!");
    }
    
    Serial.println("=== End of time fetch ===\n");
}

String getCurrentTime() {
    // Convert to 12-hour format
    int hour12 = currentHour % 12;
    if (hour12 == 0) hour12 = 12;
    
    String ampm = (currentHour >= 12) ? "PM" : "AM";
    
    char timeStr[20];
    sprintf(timeStr, "%2d:%02d %s", hour12, currentMinute, ampm.c_str());
    return String(timeStr);
}

String getCurrentDate() {
    char dateStr[20];
    sprintf(dateStr, "%s %d, %d", monthNames[currentMonth - 1], currentDay, currentYear);
    return String(dateStr);
}

// Separate function to update time (called always, even when not displaying)
void updateClockTime() {
    if (!timeInitialized) {
        return;
    }
    
    // Update seconds locally
    unsigned long currentTime = millis();
    unsigned long elapsed = currentTime - lastUpdateTime;
    
    // Handle multiple seconds if needed (e.g., if loop was delayed)
    while (elapsed >= 1000) {
        currentSecond++;
        if (currentSecond >= 60) {
            currentSecond = 0;
            currentMinute++;
            if (currentMinute >= 60) {
                currentMinute = 0;
                currentHour++;
                if (currentHour >= 24) {
                    currentHour = 0;
                }
            }
        }
        // Increment by exactly 1000ms to avoid drift
        lastUpdateTime += 1000;
        elapsed = currentTime - lastUpdateTime;
    }
    
    // Fetch from API periodically
    if (currentTime - lastFetchTime >= FETCH_INTERVAL) {
        updateClock();
    }
}

void displayClock() {
    // If time not initialized, show error
    if (!timeInitialized) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(15, 25);
        display.println("Try reset");
        display.display();
        return;
    }
    
    // Update time (this happens even when not displaying)
    updateClockTime();
    
    // Display on OLED - Modern Design
    display.clearDisplay();
    display.setTextColor(WHITE);
    
    // Draw modern frame border
    display.drawRoundRect(0, 0, 128, 64, 3, WHITE);
    
    // Draw time (medium, centered)
    display.setTextSize(2);
    String timeStr = getCurrentTime();
    int16_t x = (128 - (timeStr.length() * 12)) / 2; // Center text (size 2 = 12px per char)
    display.setCursor(x, 12);
    display.print(timeStr);
    
    // Draw seconds with WiFi icon in modern box
    display.fillRoundRect(32, 32, 64, 12, 2, WHITE);
    display.setTextColor(BLACK);
    display.setTextSize(1);
    char secStr[4];
    sprintf(secStr, ":%02d", currentSecond);
    x = (128 - (strlen(secStr) * 6)) / 2 - 6;
    display.setCursor(x, 35);
    display.print(secStr);
    
    // Draw WiFi icon in the box
    if (isWiFiConnected()) {
        int iconX = x + (strlen(secStr) * 6) + 3;
        int iconY = 35;
        // Invert WiFi icon for black background
        for (int i = 0; i < 8; i++) {
            uint8_t line = pgm_read_byte(&wifiIcon[i]);
            for (int j = 0; j < 8; j++) {
                if (line & (1 << (7 - j))) {
                    display.drawPixel(iconX + j, iconY + i, BLACK);
                }
            }
        }
    }
    display.setTextColor(WHITE); // Reset to white
    
    // Build marquee text with date and branding
    String dateStr = getCurrentDate();
    marqueeText = dateStr + " - All Services are up - Moon iOT";
    
    // Draw scrolling marquee (bottom with better spacing)
    display.setTextSize(1);
    
    // Only draw characters that are within the visible area (4 to 124 to avoid border)
    for (int i = 0; i < (int)marqueeText.length(); i++) {
        int charX = marqueeX + (i * 6);
        // Only draw if character is within visible bounds
        if (charX >= 4 && charX < 122) {
            display.setCursor(charX, 50);
            display.print(marqueeText[i]);
        }
    }
    
    // Update marquee position
    unsigned long currentMillis = millis();
    if (currentMillis - lastMarqueeUpdate >= MARQUEE_SPEED) {
        marqueeX -= 2; // Move 2 pixels left
        
        // Reset when text scrolls off the left edge
        int textWidth = marqueeText.length() * 6;
        if (marqueeX < -textWidth) {
            marqueeX = 128; // Reset to right edge
        }
        
        lastMarqueeUpdate = currentMillis;
    }
    
    display.display();
}

