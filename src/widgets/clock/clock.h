#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

// Function declarations
void initClock();
void updateClock();
void displayClock();
String getCurrentTime();
String getCurrentDate();

#endif

