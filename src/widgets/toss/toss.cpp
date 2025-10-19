#include "toss.h"
#include "../../config/display.h"

// Animation states
enum TossState {
    COIN_FLIPPING,
    COIN_RESULT,
    DICE_ROLLING,
    DICE_RESULT,
    SHOW_BOTH
};

static TossState currentState = COIN_FLIPPING;
static unsigned long lastStateChange = 0;
static unsigned long lastAnimFrame = 0;
static int animationFrame = 0;

// Results
static bool coinIsHeads = true;
static int diceValue = 1;

// Timing constants
const unsigned long FLIP_DURATION = 2000;    // 2 seconds for coin flip
const unsigned long RESULT_PAUSE = 1500;     // 1.5 seconds to show coin result
const unsigned long ROLL_DURATION = 2000;    // 2 seconds for dice roll
const unsigned long FINAL_PAUSE = 2000;      // 2 seconds showing both
const unsigned long ANIM_FRAME_TIME = 100;   // Animation frame update

void initToss() {
    randomSeed(analogRead(0));
    currentState = COIN_FLIPPING;
    lastStateChange = millis();
}

// Draw a pretty coin at position with optional rotation
void drawCoin(int x, int y, int rotation) {
    // rotation: 0 = heads (circle), 1 = side view (thin), 2 = tails (circle)
    int radius = 18;
    
    if (rotation == 1) {
        // Side view - thin ellipse
        display.drawLine(x - 4, y - radius, x - 4, y + radius, WHITE);
        display.drawLine(x + 4, y - radius, x + 4, y + radius, WHITE);
        display.drawLine(x - 4, y - radius, x + 4, y - radius, WHITE);
        display.drawLine(x - 4, y + radius, x + 4, y + radius, WHITE);
    } else {
        // Full circle view
        display.drawCircle(x, y, radius, WHITE);
        display.drawCircle(x, y, radius - 2, WHITE);
        
        if (rotation == 0) {
            // Heads - "H" in center
            display.drawLine(x - 6, y - 8, x - 6, y + 8, WHITE);
            display.drawLine(x + 6, y - 8, x + 6, y + 8, WHITE);
            display.drawLine(x - 6, y, x + 6, y, WHITE);
        } else {
            // Tails - "T" in center
            display.drawLine(x - 6, y - 8, x + 6, y - 8, WHITE);
            display.drawLine(x, y - 8, x, y + 8, WHITE);
        }
    }
}

// Draw a pretty dice at position with given face value
void drawDice(int x, int y, int value) {
    int size = 32;
    int halfSize = size / 2;
    int dotRadius = 2;
    
    // Draw dice cube
    display.drawRect(x - halfSize, y - halfSize, size, size, WHITE);
    display.drawRect(x - halfSize + 2, y - halfSize + 2, size - 4, size - 4, WHITE);
    
    // Draw dots based on value
    int dotOffset = 8;
    int centerDotX = x;
    int centerDotY = y;
    
    switch (value) {
        case 1:
            // Center dot
            display.fillCircle(centerDotX, centerDotY, dotRadius, WHITE);
            break;
            
        case 2:
            // Diagonal: top-left and bottom-right
            display.fillCircle(x - dotOffset, y - dotOffset, dotRadius, WHITE);
            display.fillCircle(x + dotOffset, y + dotOffset, dotRadius, WHITE);
            break;
            
        case 3:
            // Diagonal + center
            display.fillCircle(x - dotOffset, y - dotOffset, dotRadius, WHITE);
            display.fillCircle(centerDotX, centerDotY, dotRadius, WHITE);
            display.fillCircle(x + dotOffset, y + dotOffset, dotRadius, WHITE);
            break;
            
        case 4:
            // Four corners
            display.fillCircle(x - dotOffset, y - dotOffset, dotRadius, WHITE);
            display.fillCircle(x + dotOffset, y - dotOffset, dotRadius, WHITE);
            display.fillCircle(x - dotOffset, y + dotOffset, dotRadius, WHITE);
            display.fillCircle(x + dotOffset, y + dotOffset, dotRadius, WHITE);
            break;
            
        case 5:
            // Four corners + center
            display.fillCircle(x - dotOffset, y - dotOffset, dotRadius, WHITE);
            display.fillCircle(x + dotOffset, y - dotOffset, dotRadius, WHITE);
            display.fillCircle(centerDotX, centerDotY, dotRadius, WHITE);
            display.fillCircle(x - dotOffset, y + dotOffset, dotRadius, WHITE);
            display.fillCircle(x + dotOffset, y + dotOffset, dotRadius, WHITE);
            break;
            
        case 6:
            // Two columns of three
            display.fillCircle(x - dotOffset, y - dotOffset, dotRadius, WHITE);
            display.fillCircle(x - dotOffset, centerDotY, dotRadius, WHITE);
            display.fillCircle(x - dotOffset, y + dotOffset, dotRadius, WHITE);
            display.fillCircle(x + dotOffset, y - dotOffset, dotRadius, WHITE);
            display.fillCircle(x + dotOffset, centerDotY, dotRadius, WHITE);
            display.fillCircle(x + dotOffset, y + dotOffset, dotRadius, WHITE);
            break;
    }
}

void updateToss() {
    unsigned long currentTime = millis();
    
    // Update animation frame
    if (currentTime - lastAnimFrame >= ANIM_FRAME_TIME) {
        animationFrame++;
        lastAnimFrame = currentTime;
    }
    
    // State machine
    switch (currentState) {
        case COIN_FLIPPING:
            if (currentTime - lastStateChange >= FLIP_DURATION) {
                coinIsHeads = random(0, 2) == 0;
                currentState = COIN_RESULT;
                lastStateChange = currentTime;
                animationFrame = 0;
                Serial.print("Coin: ");
                Serial.println(coinIsHeads ? "Heads" : "Tails");
            }
            break;
            
        case COIN_RESULT:
            if (currentTime - lastStateChange >= RESULT_PAUSE) {
                currentState = DICE_ROLLING;
                lastStateChange = currentTime;
                animationFrame = 0;
            }
            break;
            
        case DICE_ROLLING:
            if (currentTime - lastStateChange >= ROLL_DURATION) {
                diceValue = random(1, 7);
                currentState = DICE_RESULT;
                lastStateChange = currentTime;
                animationFrame = 0;
                Serial.print("Dice: ");
                Serial.println(diceValue);
            }
            break;
            
        case DICE_RESULT:
            if (currentTime - lastStateChange >= FINAL_PAUSE) {
                currentState = SHOW_BOTH;
                lastStateChange = currentTime;
            }
            break;
            
        case SHOW_BOTH:
            if (currentTime - lastStateChange >= FINAL_PAUSE) {
                // Reset to start over
                currentState = COIN_FLIPPING;
                lastStateChange = currentTime;
                animationFrame = 0;
            }
            break;
    }
}

void displayToss() {
    updateToss();
    display.clearDisplay();
    
    switch (currentState) {
        case COIN_FLIPPING: {
            // Animated coin flip - cycle through rotations
            int coinX = 64;
            int coinY = 24;
            int rotation = (animationFrame / 2) % 3; // 0, 1, 2 rotation states
            
            drawCoin(coinX, coinY, rotation);
            
            // Text below
            display.setTextSize(1);
            display.setCursor(40, 50);
            display.print("FLIPPING...");
            break;
        }
        
        case COIN_RESULT: {
            // Show coin result
            int coinX = 64;
            int coinY = 24;
            
            drawCoin(coinX, coinY, coinIsHeads ? 0 : 2);
            
            // Result text
            display.setTextSize(1);
            display.setCursor(coinIsHeads ? 45 : 45, 50);
            display.print(coinIsHeads ? "HEADS!" : "TAILS!");
            break;
        }
        
        case DICE_ROLLING: {
            // Show static coin result on left, rolling dice on right
            drawCoin(32, 24, coinIsHeads ? 0 : 2);
            
            // Animated dice - cycle through random values
            int randomFace = (animationFrame % 6) + 1;
            drawDice(96, 28, randomFace);
            
            // Text below
            display.setTextSize(1);
            display.setCursor(16, 50);
            display.print(coinIsHeads ? "H" : "T");
            display.setCursor(75, 50);
            display.print("ROLLING...");
            break;
        }
        
        case DICE_RESULT:
        case SHOW_BOTH: {
            // Show both results
            drawCoin(32, 24, coinIsHeads ? 0 : 2);
            drawDice(96, 28, diceValue);
            
            // Results below
            display.setTextSize(1);
            display.setCursor(10, 50);
            display.print(coinIsHeads ? "HEADS" : "TAILS");
            display.setCursor(82, 50);
            display.print("DICE:");
            display.setCursor(94, 58);
            display.print(diceValue);
            break;
        }
    }
    
    display.display();
}

