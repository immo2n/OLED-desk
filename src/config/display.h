#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define OLED_SDA 14   // D6
#define OLED_SCL 12   // D5

void initDisplay();
extern Adafruit_SSD1306 display;

#endif