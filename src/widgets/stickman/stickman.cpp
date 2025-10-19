#include "stickman.h"
#include "animations.h"
#include "../../config/display.h"

// Stickman state
int stickmanX = 64;
int stickmanY = 32;
Animation currentAnimation = HIGH_FIVE;
Animation previousAnimation = HIGH_FIVE; // Track previous to avoid repeats
int animationFrame = 0;
unsigned long lastFrameUpdate = 0;
unsigned long lastAnimationChange = 0;
const unsigned long FRAME_INTERVAL = 150; // Frame update every 150ms
const unsigned long ANIMATION_DURATION = 5000; // Change animation every 5 seconds

void initStickman() {
    randomSeed(analogRead(0));
    currentAnimation = (Animation)random(0, 6);
}

void updateStickman() {
    unsigned long currentTime = millis();
    
    // Update animation frame
    if (currentTime - lastFrameUpdate >= FRAME_INTERVAL) {
        animationFrame++;
        if (animationFrame > 7) { // 8 frames per animation
            animationFrame = 0;
        }
        lastFrameUpdate = currentTime;
    }
    
    // Change animation randomly (but avoid immediate repeats)
    if (currentTime - lastAnimationChange >= ANIMATION_DURATION) {
        Animation newAnimation;
        int attempts = 0;
        
        // Keep trying until we get a different animation (max 10 attempts for safety)
        do {
            newAnimation = (Animation)random(0, 6);
            attempts++;
        } while (newAnimation == previousAnimation && attempts < 10);
        
        previousAnimation = currentAnimation;
        currentAnimation = newAnimation;
        animationFrame = 0;
        lastAnimationChange = currentTime;
        
        // Print what animation is playing
        const char* animNames[] = {"High Five", "Dancing Together", "Playing Catch", "Push Ups", "Tug of War", "Celebrating"};
        Serial.print("Stickman: ");
        Serial.println(animNames[currentAnimation]);
    }
}

void displayStickman() {
    updateStickman();
    
    display.clearDisplay();
    
    // Draw ground line
    display.drawLine(0, 55, 128, 55, WHITE);
    
    // Draw TWO stick men based on current animation
    switch (currentAnimation) {
        case HIGH_FIVE:
            drawHighFive(display, animationFrame);
            break;
        case DANCING_TOGETHER:
            drawDancingTogether(display, animationFrame);
            break;
        case PLAYING_CATCH:
            drawPlayingCatch(display, animationFrame);
            break;
        case PUSH_UPS:
            drawPushUps(display, animationFrame);
            break;
        case TUG_OF_WAR:
            drawTugOfWar(display, animationFrame);
            break;
        case CELEBRATION_TOGETHER:
            drawCelebrationTogether(display, animationFrame);
            break;
    }
    
    display.display();
}

