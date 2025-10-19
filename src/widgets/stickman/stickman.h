#ifndef STICKMAN_H
#define STICKMAN_H

#include <Arduino.h>

// Animation types (now for TWO stickmen!)
enum Animation {
    HIGH_FIVE,
    DANCING_TOGETHER,
    PLAYING_CATCH,
    PUSH_UPS,
    TUG_OF_WAR,
    CELEBRATION_TOGETHER
};

// Function declarations
void initStickman();
void displayStickman();
void updateStickman();

#endif

