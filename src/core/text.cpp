#include "text.h"
#include "../config/display.h"

void text(const char* text, int x, int y, int size) {
    display.clearDisplay();
    display.setTextSize(size);
    display.setTextColor(WHITE);
    display.setCursor(x, y);
    display.println(text);
    display.display();
}