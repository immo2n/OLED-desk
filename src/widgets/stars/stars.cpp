#include "stars.h"
#include "../../config/display.h"

// Star positions and states
struct Star {
    int x;
    int y;
    int brightness; // 0-3 for different sizes
    bool increasing;
};

const int NUM_STARS = 20; // Fewer but bigger stars
Star stars[NUM_STARS];
unsigned long lastBlinkTime = 0;
const unsigned long BLINK_INTERVAL = 200; // Blink every 200ms

void initStars() {
    // Initialize random star positions
    randomSeed(analogRead(0));
    
    for (int i = 0; i < NUM_STARS; i++) {
        stars[i].x = random(5, 123);
        stars[i].y = random(5, 59);
        stars[i].brightness = random(0, 4);
        stars[i].increasing = random(0, 2);
    }
}

void updateStars() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
        lastBlinkTime = currentTime;
        
        // Update star brightness (twinkling effect)
        for (int i = 0; i < NUM_STARS; i++) {
            if (stars[i].increasing) {
                stars[i].brightness++;
                if (stars[i].brightness >= 3) {
                    stars[i].increasing = false;
                }
            } else {
                stars[i].brightness--;
                if (stars[i].brightness <= 0) {
                    stars[i].increasing = true;
                }
            }
        }
    }
}

void displayStars() {
    updateStars();
    
    display.clearDisplay();
    
    // Draw stars with different sizes based on brightness - BIGGER!
    for (int i = 0; i < NUM_STARS; i++) {
        int x = stars[i].x;
        int y = stars[i].y;
        int b = stars[i].brightness;
        
        if (b == 0) {
            // Small star (plus sign)
            display.drawPixel(x, y, WHITE);
            display.drawPixel(x-1, y, WHITE);
            display.drawPixel(x+1, y, WHITE);
            display.drawPixel(x, y-1, WHITE);
            display.drawPixel(x, y+1, WHITE);
        } else if (b == 1) {
            // Medium star (larger cross)
            display.drawPixel(x, y, WHITE);
            display.drawPixel(x-1, y, WHITE);
            display.drawPixel(x+1, y, WHITE);
            display.drawPixel(x-2, y, WHITE);
            display.drawPixel(x+2, y, WHITE);
            display.drawPixel(x, y-1, WHITE);
            display.drawPixel(x, y+1, WHITE);
            display.drawPixel(x, y-2, WHITE);
            display.drawPixel(x, y+2, WHITE);
            display.drawPixel(x-1, y-1, WHITE);
            display.drawPixel(x+1, y-1, WHITE);
            display.drawPixel(x-1, y+1, WHITE);
            display.drawPixel(x+1, y+1, WHITE);
        } else if (b == 2) {
            // Big star (8-point star)
            // Center
            display.fillCircle(x, y, 1, WHITE);
            // Cross
            display.drawLine(x-3, y, x+3, y, WHITE);
            display.drawLine(x, y-3, x, y+3, WHITE);
            // Diagonals
            display.drawPixel(x-2, y-2, WHITE);
            display.drawPixel(x+2, y-2, WHITE);
            display.drawPixel(x-2, y+2, WHITE);
            display.drawPixel(x+2, y+2, WHITE);
            display.drawPixel(x-1, y-1, WHITE);
            display.drawPixel(x+1, y-1, WHITE);
            display.drawPixel(x-1, y+1, WHITE);
            display.drawPixel(x+1, y+1, WHITE);
        } else {
            // Brightest star (large 8-point with glow)
            // Center glow
            display.fillCircle(x, y, 2, WHITE);
            // Long rays
            display.drawLine(x-4, y, x+4, y, WHITE);
            display.drawLine(x, y-4, x, y+4, WHITE);
            // Diagonal rays
            display.drawLine(x-3, y-3, x-2, y-2, WHITE);
            display.drawLine(x+3, y-3, x+2, y-2, WHITE);
            display.drawLine(x-3, y+3, x-2, y+2, WHITE);
            display.drawLine(x+3, y+3, x+2, y+2, WHITE);
            // Outer glow points
            display.drawPixel(x-4, y-1, WHITE);
            display.drawPixel(x-4, y+1, WHITE);
            display.drawPixel(x+4, y-1, WHITE);
            display.drawPixel(x+4, y+1, WHITE);
            display.drawPixel(x-1, y-4, WHITE);
            display.drawPixel(x+1, y-4, WHITE);
            display.drawPixel(x-1, y+4, WHITE);
            display.drawPixel(x+1, y+4, WHITE);
        }
    }
    
    display.display();
}

