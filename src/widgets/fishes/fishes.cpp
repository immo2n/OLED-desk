#include "fishes.h"
#include "../../config/display.h"

// Fish structure
struct Fish {
    int x;
    int y;
    int speed;
    int size;      // 1=small, 2=medium, 3=large
    bool direction; // true=right, false=left
    int tailWag;    // Animation frame
};

// Bubble structure
struct Bubble {
    int x;
    int y;
    int speed;
    bool active;
};

const int NUM_FISH = 4;  // Fewer fish, but bigger
const int NUM_BUBBLES = 8;
Fish fishes[NUM_FISH];
Bubble bubbles[NUM_BUBBLES];

unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 80; // Update every 80ms

void initFishes() {
    randomSeed(analogRead(0));
    
    // Initialize fish at random positions
    for (int i = 0; i < NUM_FISH; i++) {
        fishes[i].x = random(-20, 128);
        fishes[i].y = random(15, 45);
        fishes[i].speed = random(1, 3);
        fishes[i].size = random(2, 4); // Size 2-3 only (medium and large)
        fishes[i].direction = random(0, 2);
        fishes[i].tailWag = 0;
    }
    
    // Initialize bubbles
    for (int i = 0; i < NUM_BUBBLES; i++) {
        bubbles[i].x = random(0, 128);
        bubbles[i].y = random(20, 64);
        bubbles[i].speed = random(1, 3);
        bubbles[i].active = random(0, 2);
    }
}

void drawFish(int x, int y, int size, bool direction, int tailWag) {
    // Bigger, more detailed fish designs
    if (size == 2) {
        // Medium fish (20x14) - BIG!
        if (direction) {
            // Body - larger filled area
            display.fillCircle(x, y, 5, WHITE);
            display.fillCircle(x+4, y, 4, WHITE);
            display.fillCircle(x+7, y, 3, WHITE);
            display.drawPixel(x+9, y, WHITE);
            // Eye (larger with highlight)
            display.fillCircle(x+4, y-2, 2, BLACK);
            display.drawPixel(x+4, y-3, WHITE);
            // Fins (bigger)
            display.drawLine(x, y+5, x-1, y+6, WHITE);
            display.drawLine(x+1, y+6, x+1, y+7, WHITE);
            display.drawLine(x, y-5, x-1, y-6, WHITE);
            display.drawLine(x+1, y-6, x+1, y-7, WHITE);
            // Tail (bigger, animated)
            display.drawLine(x-5, y-4, x-7, y-6, WHITE);
            display.drawLine(x-5, y-3, x-8, y-5, WHITE);
            display.drawLine(x-5, y-2, x-8, y-4, WHITE);
            display.drawLine(x-5, y-1, x-9, y-2, WHITE);
            display.drawLine(x-5, y, x-10, y, WHITE);
            display.drawLine(x-5, y+1, x-9, y+2, WHITE);
            display.drawLine(x-5, y+2, x-8, y+4, WHITE);
            display.drawLine(x-5, y+3, x-8, y+5, WHITE);
            display.drawLine(x-5, y+4, x-7, y+6, WHITE);
            if (tailWag % 2 == 0) {
                display.drawPixel(x-9, y-6, WHITE);
                display.drawPixel(x-10, y-5, WHITE);
            } else {
                display.drawPixel(x-9, y+6, WHITE);
                display.drawPixel(x-10, y+5, WHITE);
            }
        } else {
            // Body - larger filled area
            display.fillCircle(x, y, 5, WHITE);
            display.fillCircle(x-4, y, 4, WHITE);
            display.fillCircle(x-7, y, 3, WHITE);
            display.drawPixel(x-9, y, WHITE);
            // Eye (larger with highlight)
            display.fillCircle(x-4, y-2, 2, BLACK);
            display.drawPixel(x-4, y-3, WHITE);
            // Fins (bigger)
            display.drawLine(x, y+5, x+1, y+6, WHITE);
            display.drawLine(x-1, y+6, x-1, y+7, WHITE);
            display.drawLine(x, y-5, x+1, y-6, WHITE);
            display.drawLine(x-1, y-6, x-1, y-7, WHITE);
            // Tail (bigger, animated)
            display.drawLine(x+5, y-4, x+7, y-6, WHITE);
            display.drawLine(x+5, y-3, x+8, y-5, WHITE);
            display.drawLine(x+5, y-2, x+8, y-4, WHITE);
            display.drawLine(x+5, y-1, x+9, y-2, WHITE);
            display.drawLine(x+5, y, x+10, y, WHITE);
            display.drawLine(x+5, y+1, x+9, y+2, WHITE);
            display.drawLine(x+5, y+2, x+8, y+4, WHITE);
            display.drawLine(x+5, y+3, x+8, y+5, WHITE);
            display.drawLine(x+5, y+4, x+7, y+6, WHITE);
            if (tailWag % 2 == 0) {
                display.drawPixel(x+9, y-6, WHITE);
                display.drawPixel(x+10, y-5, WHITE);
            } else {
                display.drawPixel(x+9, y+6, WHITE);
                display.drawPixel(x+10, y+5, WHITE);
            }
        }
    } else {
        // Extra Large fish (26x16) - MASSIVE!
        if (direction) {
            // Body - massive with mouth
            display.fillCircle(x, y, 6, WHITE);
            display.fillCircle(x+5, y, 5, WHITE);
            display.fillCircle(x+9, y, 4, WHITE);
            display.drawPixel(x+12, y, WHITE);
            display.drawPixel(x+13, y, WHITE);
            // Mouth
            display.drawLine(x+11, y-1, x+12, y-2, WHITE);
            display.drawLine(x+11, y+1, x+12, y+2, WHITE);
            // Eye (huge with highlight)
            display.fillCircle(x+6, y-2, 2, BLACK);
            display.fillCircle(x+7, y-3, 1, WHITE);
            // Fins (much bigger)
            display.drawLine(x, y+6, x-2, y+7, WHITE);
            display.drawLine(x+1, y+7, x+2, y+8, WHITE);
            display.drawLine(x, y-6, x-2, y-7, WHITE);
            display.drawLine(x+1, y-7, x+2, y-8, WHITE);
            // Tail (massive and fancier)
            display.drawLine(x-6, y-5, x-10, y-7, WHITE);
            display.drawLine(x-6, y-4, x-10, y-6, WHITE);
            display.drawLine(x-6, y-3, x-11, y-5, WHITE);
            display.drawLine(x-6, y-2, x-12, y-3, WHITE);
            display.drawLine(x-6, y-1, x-13, y-1, WHITE);
            display.drawLine(x-6, y, x-14, y, WHITE);
            display.drawLine(x-6, y+1, x-13, y+1, WHITE);
            display.drawLine(x-6, y+2, x-12, y+3, WHITE);
            display.drawLine(x-6, y+3, x-11, y+5, WHITE);
            display.drawLine(x-6, y+4, x-10, y+6, WHITE);
            display.drawLine(x-6, y+5, x-10, y+7, WHITE);
            if (tailWag % 2 == 0) {
                display.drawPixel(x-11, y-7, WHITE);
                display.drawPixel(x-12, y-6, WHITE);
            } else {
                display.drawPixel(x-11, y+7, WHITE);
                display.drawPixel(x-12, y+6, WHITE);
            }
        } else {
            // Body - massive with mouth
            display.fillCircle(x, y, 6, WHITE);
            display.fillCircle(x-5, y, 5, WHITE);
            display.fillCircle(x-9, y, 4, WHITE);
            display.drawPixel(x-12, y, WHITE);
            display.drawPixel(x-13, y, WHITE);
            // Mouth
            display.drawLine(x-11, y-1, x-12, y-2, WHITE);
            display.drawLine(x-11, y+1, x-12, y+2, WHITE);
            // Eye (huge with highlight)
            display.fillCircle(x-6, y-2, 2, BLACK);
            display.fillCircle(x-7, y-3, 1, WHITE);
            // Fins (much bigger)
            display.drawLine(x, y+6, x+2, y+7, WHITE);
            display.drawLine(x-1, y+7, x-2, y+8, WHITE);
            display.drawLine(x, y-6, x+2, y-7, WHITE);
            display.drawLine(x-1, y-7, x-2, y-8, WHITE);
            // Tail (massive and fancier)
            display.drawLine(x+6, y-5, x+10, y-7, WHITE);
            display.drawLine(x+6, y-4, x+10, y-6, WHITE);
            display.drawLine(x+6, y-3, x+11, y-5, WHITE);
            display.drawLine(x+6, y-2, x+12, y-3, WHITE);
            display.drawLine(x+6, y-1, x+13, y-1, WHITE);
            display.drawLine(x+6, y, x+14, y, WHITE);
            display.drawLine(x+6, y+1, x+13, y+1, WHITE);
            display.drawLine(x+6, y+2, x+12, y+3, WHITE);
            display.drawLine(x+6, y+3, x+11, y+5, WHITE);
            display.drawLine(x+6, y+4, x+10, y+6, WHITE);
            display.drawLine(x+6, y+5, x+10, y+7, WHITE);
            if (tailWag % 2 == 0) {
                display.drawPixel(x+11, y-7, WHITE);
                display.drawPixel(x+12, y-6, WHITE);
            } else {
                display.drawPixel(x+11, y+7, WHITE);
                display.drawPixel(x+12, y+6, WHITE);
            }
        }
    }
}

void updateFishes() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
        lastUpdate = currentTime;
        
        // Update fish positions
        for (int i = 0; i < NUM_FISH; i++) {
            if (fishes[i].direction) {
                fishes[i].x += fishes[i].speed;
                if (fishes[i].x > 145) {
                    fishes[i].x = -25;
                    fishes[i].y = random(15, 45);
                    fishes[i].size = random(2, 4); // Regenerate size
                }
            } else {
                fishes[i].x -= fishes[i].speed;
                if (fishes[i].x < -25) {
                    fishes[i].x = 145;
                    fishes[i].y = random(15, 45);
                    fishes[i].size = random(2, 4); // Regenerate size
                }
            }
            
            // Update tail animation
            fishes[i].tailWag++;
        }
        
        // Update bubbles
        for (int i = 0; i < NUM_BUBBLES; i++) {
            if (bubbles[i].active) {
                bubbles[i].y -= bubbles[i].speed;
                
                // Reset bubble when it reaches top
                if (bubbles[i].y < 5) {
                    bubbles[i].y = random(50, 64);
                    bubbles[i].x = random(0, 128);
                    bubbles[i].active = random(0, 3) > 0; // 2/3 chance to stay active
                }
            } else {
                // Random chance to activate
                if (random(0, 50) == 0) {
                    bubbles[i].active = true;
                    bubbles[i].y = random(50, 64);
                    bubbles[i].x = random(0, 128);
                }
            }
        }
    }
}

void displayFishes() {
    updateFishes();
    
    display.clearDisplay();
    
    // Draw wavy ground
    for (int x = 0; x < 128; x += 4) {
        int waveY = 58 + (sin(x * 0.1 + millis() * 0.001) * 3);
        display.drawPixel(x, waveY, WHITE);
        display.drawPixel(x+1, waveY, WHITE);
    }
    
    // Draw bubbles
    for (int i = 0; i < NUM_BUBBLES; i++) {
        if (bubbles[i].active) {
            display.drawCircle(bubbles[i].x, bubbles[i].y, 1, WHITE);
        }
    }
    
    // Draw fish
    for (int i = 0; i < NUM_FISH; i++) {
        drawFish(fishes[i].x, fishes[i].y, fishes[i].size, fishes[i].direction, fishes[i].tailWag);
    }
    
    // Draw "Aquarium" text
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(25, 2);
    display.print("~ Fish World ~");
    
    display.display();
}

