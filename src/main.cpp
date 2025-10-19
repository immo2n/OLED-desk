#include <Arduino.h>
#include "config/display.h"
#include "config/wifi.h"
#include "widgets/clock/clock.h"
#include "widgets/stars/stars.h"
#include "widgets/fishes/fishes.h"
#include "widgets/stickman/stickman.h"
#include "widgets/toss/toss.h"

// Widget switching
enum Widget { CLOCK, FISHES, STARS, STICKMAN, TOSS };
Widget currentWidget = CLOCK;
unsigned long lastSwitchTime = 0;
const unsigned long CLOCK_DURATION = 20000;   // 20 seconds
const unsigned long WIDGET_DURATION = 10000;  // 10 seconds for each widget

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
    
    // Initialize stickman
    initStickman();
    
    // Initialize toss
    initToss();
    
    // Start with clock
    lastSwitchTime = millis();
}

void loop() {
    unsigned long currentTime = millis();
    
    // Check if it's time to switch widgets
    // Pattern: Clock 20s -> Random Widget 10s -> Clock 20s -> Random Widget 10s -> repeat
    if (currentWidget == CLOCK) {
        if (currentTime - lastSwitchTime >= CLOCK_DURATION) {
            // Randomly pick a widget (Fish, Stars, Stickman, or Toss)
            Widget widgets[] = {FISHES, STARS, STICKMAN, TOSS};
            int randomIndex = random(0, 4);
            currentWidget = widgets[randomIndex];
            lastSwitchTime = currentTime;
            
            const char* widgetNames[] = {"Fishes", "Stars", "Stickman", "Toss"};
            Serial.print("Switching to ");
            Serial.println(widgetNames[randomIndex]);
        }
    } else {
        // Any widget returns to Clock after 10 seconds
        if (currentTime - lastSwitchTime >= WIDGET_DURATION) {
            currentWidget = CLOCK;
            lastSwitchTime = currentTime;
            Serial.println("Switching to Clock");
        }
    }
    
    // Always update clock time in background
    updateClockTime();
    
    // Display current widget
    if (currentWidget == CLOCK) {
        displayClock();
    } else if (currentWidget == FISHES) {
        displayFishes();
    } else if (currentWidget == STARS) {
        displayStars();
    } else if (currentWidget == STICKMAN) {
        displayStickman();
    } else {
        displayToss();
    }
    
    delay(50); // Small delay for smooth animation
}