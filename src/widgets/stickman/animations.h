#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Adafruit_SSD1306.h>

// Helper function to draw a single stickman
void drawStickman(Adafruit_SSD1306& display, int x, int y, 
                  int leftArmAngle, int rightArmAngle, 
                  int leftLegAngle, int rightLegAngle, 
                  int headOffset = 0, int bodyTilt = 0);

// Two-person animation drawing functions
void drawHighFive(Adafruit_SSD1306& display, int frame);
void drawDancingTogether(Adafruit_SSD1306& display, int frame);
void drawPlayingCatch(Adafruit_SSD1306& display, int frame);
void drawRacing(Adafruit_SSD1306& display, int frame);
void drawTugOfWar(Adafruit_SSD1306& display, int frame);
void drawCelebrationTogether(Adafruit_SSD1306& display, int frame);

#endif

