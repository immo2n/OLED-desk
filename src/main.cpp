#include <Arduino.h>
#include "config/display.h"
#include "config/wifi.h"
#include "widgets/clock/clock.h"
#include "widgets/stars/stars.h"

// Widget switching
enum Widget { CLOCK, STARS };
Widget currentWidget = CLOCK;
unsigned long lastSwitchTime = 0;
const unsigned long CLOCK_DURATION = 20000; // 10 seconds
const unsigned long STARS_DURATION = 5000;  // 5 seconds

void setup() {
    Serial.begin(115200);
    
    // Initialize display
    initDisplay();
    
    // Connect to WiFi
    initWiFi();
    
    // Initialize clock (fetches time from API)
    initClock();
    
    // Initialize stars
    initStars();
    
    // Initialize fishes
    initFishes();
    
    // Start with clock
    lastSwitchTime = millis();
}

void loop() {
    unsigned long currentTime = millis();
    
    // Check if it's time to switch widgets
    if (currentWidget == CLOCK) {
        if (currentTime - lastSwitchTime >= CLOCK_DURATION) {
            currentWidget = STARS;
            lastSwitchTime = currentTime;
            Serial.println("Switching to Stars");
        }
    } else {
        if (currentTime - lastSwitchTime >= STARS_DURATION) {
            currentWidget = CLOCK;
            lastSwitchTime = currentTime;
            Serial.println("Switching to Clock");
        }
    }
    
    // Display current widget
    if (currentWidget == CLOCK) {
        displayClock();
    } else {
        displayStars();
    }
    
    delay(50); // Small delay for smooth animation
}