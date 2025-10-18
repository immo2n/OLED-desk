#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

// Function declarations
void initClock();
void updateClock();
void updateClockTime();  // Update time in background
void displayClock();
String getCurrentTime();
String getCurrentDate();

#endif

