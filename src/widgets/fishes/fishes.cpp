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

const int NUM_FISH = 5;
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
        fishes[i].y = random(10, 50);
        fishes[i].speed = random(1, 4);
        fishes[i].size = random(1, 4);
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
    // Simple fish design
    if (size == 1) {
        // Small fish (5x3)
        if (direction) {
            // Facing right
            display.drawPixel(x, y, WHITE);
            display.drawPixel(x+1, y-1, WHITE);
            display.drawPixel(x+1, y, WHITE);
            display.drawPixel(x+1, y+1, WHITE);
            display.drawPixel(x+2, y, WHITE);
            // Tail (wagging)
            display.drawPixel(x-1, y + (tailWag % 2 == 0 ? -1 : 1), WHITE);
        } else {
            // Facing left
            display.drawPixel(x, y, WHITE);
            display.drawPixel(x-1, y-1, WHITE);
            display.drawPixel(x-1, y, WHITE);
            display.drawPixel(x-1, y+1, WHITE);
            display.drawPixel(x-2, y, WHITE);
            // Tail
            display.drawPixel(x+1, y + (tailWag % 2 == 0 ? -1 : 1), WHITE);
        }
    } else if (size == 2) {
        // Medium fish (7x5)
        if (direction) {
            // Body
            display.fillCircle(x, y, 2, WHITE);
            display.drawPixel(x+2, y, WHITE);
            display.drawPixel(x+3, y, WHITE);
            // Eye
            display.drawPixel(x+1, y-1, BLACK);
            // Tail
            display.drawPixel(x-2, y-1, WHITE);
            display.drawPixel(x-2, y, WHITE);
            display.drawPixel(x-2, y+1, WHITE);
            if (tailWag % 2 == 0) {
                display.drawPixel(x-3, y-1, WHITE);
            } else {
                display.drawPixel(x-3, y+1, WHITE);
            }
        } else {
            // Body
            display.fillCircle(x, y, 2, WHITE);
            display.drawPixel(x-2, y, WHITE);
            display.drawPixel(x-3, y, WHITE);
            // Eye
            display.drawPixel(x-1, y-1, BLACK);
            // Tail
            display.drawPixel(x+2, y-1, WHITE);
            display.drawPixel(x+2, y, WHITE);
            display.drawPixel(x+2, y+1, WHITE);
            if (tailWag % 2 == 0) {
                display.drawPixel(x+3, y-1, WHITE);
            } else {
                display.drawPixel(x+3, y+1, WHITE);
            }
        }
    } else {
        // Large fish (9x7)
        if (direction) {
            // Body
            display.fillCircle(x, y, 3, WHITE);
            display.drawPixel(x+3, y, WHITE);
            display.drawPixel(x+4, y, WHITE);
            // Eye
            display.drawPixel(x+2, y-1, BLACK);
            // Fins
            display.drawPixel(x, y+3, WHITE);
            display.drawPixel(x, y-3, WHITE);
            // Tail
            display.drawLine(x-3, y-2, x-4, y-3, WHITE);
            display.drawLine(x-3, y, x-4, y, WHITE);
            display.drawLine(x-3, y+2, x-4, y+3, WHITE);
            if (tailWag % 2 == 0) {
                display.drawPixel(x-5, y-3, WHITE);
            } else {
                display.drawPixel(x-5, y+3, WHITE);
            }
        } else {
            // Body
            display.fillCircle(x, y, 3, WHITE);
            display.drawPixel(x-3, y, WHITE);
            display.drawPixel(x-4, y, WHITE);
            // Eye
            display.drawPixel(x-2, y-1, BLACK);
            // Fins
            display.drawPixel(x, y+3, WHITE);
            display.drawPixel(x, y-3, WHITE);
            // Tail
            display.drawLine(x+3, y-2, x+4, y-3, WHITE);
            display.drawLine(x+3, y, x+4, y, WHITE);
            display.drawLine(x+3, y+2, x+4, y+3, WHITE);
            if (tailWag % 2 == 0) {
                display.drawPixel(x+5, y-3, WHITE);
            } else {
                display.drawPixel(x+5, y+3, WHITE);
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
                if (fishes[i].x > 140) {
                    fishes[i].x = -20;
                    fishes[i].y = random(10, 50);
                }
            } else {
                fishes[i].x -= fishes[i].speed;
                if (fishes[i].x < -20) {
                    fishes[i].x = 140;
                    fishes[i].y = random(10, 50);
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
    display.setCursor(40, 2);
    display.print("~ Aquarium ~");
    
    display.display();
}

