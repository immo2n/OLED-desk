#include "animations.h"

// Helper function to draw a basic stickman at any position with any pose
void drawStickman(Adafruit_SSD1306& display, int x, int y, 
                  int leftArmAngle, int rightArmAngle, 
                  int leftLegAngle, int rightLegAngle, 
                  int headOffset, int bodyTilt) {
    // Head
    display.fillCircle(x, y - 12 + headOffset, 4, WHITE);
    
    // Body
    display.drawLine(x, y - 8 + headOffset, x + bodyTilt, y + 2, WHITE);
    
    // Arms
    display.drawLine(x, y - 6 + headOffset, x - 7, y - 6 + headOffset + leftArmAngle, WHITE);
    display.drawLine(x, y - 6 + headOffset, x + 7, y - 6 + headOffset + rightArmAngle, WHITE);
    
    // Legs
    display.drawLine(x + bodyTilt, y + 2, x - 5, y + 12 + leftLegAngle, WHITE);
    display.drawLine(x + bodyTilt, y + 2, x + 5, y + 12 + rightLegAngle, WHITE);
}

void drawHighFive(Adafruit_SSD1306& display, int frame) {
    int x1 = 35;
    int x2 = 93;
    int y = 32;
    
    // Both running toward each other, then high five
    if (frame < 4) {
        // Running toward each other
        x1 = 20 + (frame * 3);
        x2 = 108 - (frame * 3);
        
        // Stickman 1 (left, running right)
        int legSwing = (frame % 2 == 0) ? 4 : -4;
        drawStickman(display, x1, y, -4, 4, legSwing, -legSwing, 0, 0);
        
        // Stickman 2 (right, running left)
        drawStickman(display, x2, y, 4, -4, -legSwing, legSwing, 0, 0);
    } else {
        // High five! (arms extended toward each other)
        drawStickman(display, x1, y, -2, -10, 0, 0, 0, 0);
        drawStickman(display, x2, y, -10, -2, 0, 0, 0, 0);
        
        // High five impact (star effect when frame == 4)
        if (frame == 4) {
            int midX = 64;
            int midY = 22;
            display.drawPixel(midX, midY - 3, WHITE);
            display.drawPixel(midX, midY + 3, WHITE);
            display.drawPixel(midX - 3, midY, WHITE);
            display.drawPixel(midX + 3, midY, WHITE);
            display.drawPixel(midX - 2, midY - 2, WHITE);
            display.drawPixel(midX + 2, midY - 2, WHITE);
            display.drawPixel(midX - 2, midY + 2, WHITE);
            display.drawPixel(midX + 2, midY + 2, WHITE);
        }
    }
}

void drawDancingTogether(Adafruit_SSD1306& display, int frame) {
    int x1 = 40;
    int x2 = 88;
    int y = 32;
    
    // Mirror dance moves
    bool leftHigh = (frame % 4 < 2);
    
    if (leftHigh) {
        // Left guy: left arm up, right arm down
        drawStickman(display, x1, y, -10, 2, 2, -2, (frame % 2) * -2, 0);
        // Right guy: right arm up, left arm down
        drawStickman(display, x2, y, 2, -10, -2, 2, (frame % 2) * -2, 0);
    } else {
        // Opposite
        drawStickman(display, x1, y, 2, -10, -2, 2, (frame % 2) * -2, 0);
        drawStickman(display, x2, y, -10, 2, 2, -2, (frame % 2) * -2, 0);
    }
    
    // Music notes
    if (frame % 2 == 0) {
        display.drawCircle(64, 10, 2, WHITE);
        display.drawLine(66, 10, 66, 5, WHITE);
    }
}

void drawPlayingCatch(Adafruit_SSD1306& display, int frame) {
    int x1 = 25;
    int x2 = 103;
    int y = 32;
    
    // Ball position (travels between them)
    int ballX, ballY;
    bool thrower1 = (frame / 4) % 2 == 0; // Who's throwing
    
    if (thrower1) {
        // Ball travels from left to right
        int progress = frame % 4;
        ballX = x1 + ((x2 - x1) * progress / 4);
        ballY = y - 8 - abs(progress - 2) * 8; // Arc
        
        if (progress == 0) {
            // Guy 1 throwing
            drawStickman(display, x1, y, -4, -12, 0, 0, 0, 0);
            drawStickman(display, x2, y, 4, 4, 0, 0, 0, 0);
        } else if (progress == 3) {
            // Guy 2 catching
            drawStickman(display, x1, y, -4, 4, 0, 0, 0, 0);
            drawStickman(display, x2, y, -12, -4, 0, 0, 0, 0);
        } else {
            // Mid-throw
            drawStickman(display, x1, y, -4, 4, 0, 0, 0, 0);
            drawStickman(display, x2, y, 4, 4, 0, 0, 0, 0);
        }
    } else {
        // Ball travels from right to left
        int progress = frame % 4;
        ballX = x2 - ((x2 - x1) * progress / 4);
        ballY = y - 8 - abs(progress - 2) * 8;
        
        if (progress == 0) {
            // Guy 2 throwing
            drawStickman(display, x1, y, 4, 4, 0, 0, 0, 0);
            drawStickman(display, x2, y, -12, -4, 0, 0, 0, 0);
        } else if (progress == 3) {
            // Guy 1 catching
            drawStickman(display, x1, y, -12, -4, 0, 0, 0, 0);
            drawStickman(display, x2, y, -4, 4, 0, 0, 0, 0);
        } else {
            // Mid-throw
            drawStickman(display, x1, y, 4, 4, 0, 0, 0, 0);
            drawStickman(display, x2, y, 4, 4, 0, 0, 0, 0);
        }
    }
    
    // Draw the ball
    display.fillCircle(ballX, ballY, 2, WHITE);
}

void drawRacing(Adafruit_SSD1306& display, int frame) {
    int y = 32;
    
    // Both running across screen
    int x1 = 10 + (frame * 8);
    int x2 = 15 + (frame * 7); // Slightly slower
    
    if (x1 > 128 && x2 > 128) {
        x1 = 10;
        x2 = 15;
    }
    
    // Running animation
    int legSwing1 = (frame % 2 == 0) ? 6 : -6;
    int legSwing2 = ((frame + 1) % 2 == 0) ? 6 : -6;
    
    drawStickman(display, x1, y, -4, 4, legSwing1, -legSwing1, 0, 0);
    drawStickman(display, x2, y, -4, 4, legSwing2, -legSwing2, 0, 0);
    
    // Finish line
    display.drawLine(115, 20, 115, 55, WHITE);
    display.drawLine(120, 20, 120, 55, WHITE);
}

void drawTugOfWar(Adafruit_SSD1306& display, int frame) {
    int x1 = 45 - (frame % 4);
    int x2 = 83 + (frame % 4);
    int y = 34;
    
    // Both pulling rope
    int pullAngle = (frame % 4 < 2) ? -3 : 3;
    
    // Stickman 1 (pulling left)
    drawStickman(display, x1, y, -10, -8, pullAngle, pullAngle, 0, pullAngle);
    
    // Stickman 2 (pulling right)
    drawStickman(display, x2, y, -8, -10, pullAngle, pullAngle, 0, -pullAngle);
    
    // Rope
    display.drawLine(x1 + 7, y - 6, x2 - 7, y - 6, WHITE);
    
    // Effort lines
    if (frame % 2 == 0) {
        display.drawPixel(x1 - 5, y - 14, WHITE);
        display.drawPixel(x1 - 7, y - 16, WHITE);
        display.drawPixel(x2 + 5, y - 14, WHITE);
        display.drawPixel(x2 + 7, y - 16, WHITE);
    }
}

void drawCelebrationTogether(Adafruit_SSD1306& display, int frame) {
    int x1 = 45;
    int x2 = 83;
    int y = 34;
    
    // Both jumping and celebrating
    int jump1 = (frame % 4 < 2) ? -4 : 0;
    int jump2 = ((frame + 2) % 4 < 2) ? -4 : 0; // Offset jump
    
    // Both with arms up
    drawStickman(display, x1, y + jump1, -12, -12, jump1, jump1, 0, 0);
    drawStickman(display, x2, y + jump2, -12, -12, jump2, jump2, 0, 0);
    
    // Confetti everywhere!
    for (int i = 0; i < 10; i++) {
        int confettiX = 20 + (i * 9) + (frame % 3);
        int confettiY = 8 + random(0, 15) + (frame % 4) * 2;
        if (confettiY < 50) {
            display.drawPixel(confettiX, confettiY, WHITE);
            display.drawPixel(confettiX + 1, confettiY + 1, WHITE);
        }
    }
    
    // Happy text
    display.setTextSize(1);
    display.setCursor(48, 2);
    display.print("YEAH!");
}
