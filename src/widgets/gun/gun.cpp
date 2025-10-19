#include "gun.h"
#include "../../config/display.h"

// Animation states
enum GunState {
    READY,
    FIRING,
    BULLET_FLYING,
    IMPACT,
    SMOKE_CLEAR
};

static GunState currentState = READY;
static unsigned long lastStateChange = 0;
static unsigned long lastAnimFrame = 0;
static int animationFrame = 0;

// Bullet position
static int bulletX = 0;
static int bulletY = 0;

// Smoke particles
struct SmokeParticle {
    int x, y;
    int life;
};
static SmokeParticle smoke[5];

// Timing constants
const unsigned long READY_TIME = 800;        // 0.8s ready pose
const unsigned long FIRING_TIME = 200;       // 0.2s muzzle flash
const unsigned long BULLET_TIME = 1500;      // 1.5s bullet travel
const unsigned long IMPACT_TIME = 400;       // 0.4s impact effect
const unsigned long SMOKE_TIME = 800;        // 0.8s smoke clearing
const unsigned long ANIM_FRAME_TIME = 50;    // 50ms per frame

void initGun() {
    currentState = READY;
    lastStateChange = millis();
    animationFrame = 0;
    
    // Initialize smoke particles
    for (int i = 0; i < 5; i++) {
        smoke[i].life = 0;
    }
}

// Draw a pistol
void drawPistol(int x, int y, bool recoil) {
    int recoilOffset = recoil ? -3 : 0;
    
    // Barrel
    display.fillRect(x + recoilOffset, y + 5, 20, 4, WHITE);
    
    // Slide
    display.drawRect(x - 8 + recoilOffset, y + 3, 15, 8, WHITE);
    display.fillRect(x - 7 + recoilOffset, y + 4, 13, 6, WHITE);
    
    // Grip
    display.drawLine(x - 8, y + 10, x - 8, y + 18, WHITE);
    display.drawLine(x - 8, y + 18, x - 4, y + 18, WHITE);
    display.drawLine(x - 4, y + 18, x - 2, y + 11, WHITE);
    display.drawLine(x - 2, y + 11, x - 5, y + 11, WHITE);
    display.drawLine(x - 5, y + 11, x - 8, y + 10, WHITE);
    
    // Trigger
    display.drawPixel(x - 5, y + 12, WHITE);
    display.drawPixel(x - 4, y + 13, WHITE);
    
    // Sight
    display.drawLine(x + 4 + recoilOffset, y + 2, x + 4 + recoilOffset, y + 3, WHITE);
}

// Draw muzzle flash
void drawMuzzleFlash(int x, int y, int frame) {
    int size = (frame % 2) ? 8 : 6;
    
    // Flash star pattern
    for (int i = 0; i < 8; i++) {
        int angle = i * 45;
        int len = size - (i % 2) * 2;
        float rad = angle * PI / 180.0;
        int dx = cos(rad) * len;
        int dy = sin(rad) * len;
        display.drawLine(x, y, x + dx, y + dy, WHITE);
    }
    
    // Bright center
    display.fillCircle(x, y, 2, WHITE);
}

// Draw bullet
void drawBullet(int x, int y) {
    display.fillCircle(x, y, 2, WHITE);
    display.drawCircle(x, y, 3, WHITE);
    
    // Motion trails
    display.drawPixel(x - 3, y, WHITE);
    display.drawPixel(x - 5, y, WHITE);
    display.drawPixel(x - 7, y - 1, WHITE);
    display.drawPixel(x - 7, y + 1, WHITE);
}

// Draw target
void drawTarget(int x, int y) {
    display.drawCircle(x, y, 10, WHITE);
    display.drawCircle(x, y, 6, WHITE);
    display.drawCircle(x, y, 3, WHITE);
    display.fillCircle(x, y, 1, WHITE);
}

// Draw impact effect
void drawImpact(int x, int y, int frame) {
    int size = 8 + frame * 2;
    
    // Explosion lines radiating out
    for (int i = 0; i < 12; i++) {
        int angle = i * 30;
        float rad = angle * PI / 180.0;
        int startLen = 2;
        int endLen = size;
        int x1 = x + cos(rad) * startLen;
        int y1 = y + sin(rad) * startLen;
        int x2 = x + cos(rad) * endLen;
        int y2 = y + sin(rad) * endLen;
        display.drawLine(x1, y1, x2, y2, WHITE);
    }
    
    // Debris particles
    for (int i = 0; i < 6; i++) {
        int px = x + random(-12, 12);
        int py = y + random(-12, 12);
        display.drawPixel(px, py, WHITE);
        display.drawPixel(px + 1, py, WHITE);
    }
}

void updateGun() {
    unsigned long currentTime = millis();
    
    // Update animation frame
    if (currentTime - lastAnimFrame >= ANIM_FRAME_TIME) {
        animationFrame++;
        lastAnimFrame = currentTime;
        
        // Update smoke particles
        for (int i = 0; i < 5; i++) {
            if (smoke[i].life > 0) {
                smoke[i].x += 1;
                smoke[i].y += random(-1, 2);
                smoke[i].life--;
            }
        }
    }
    
    // State machine
    switch (currentState) {
        case READY:
            if (currentTime - lastStateChange >= READY_TIME) {
                currentState = FIRING;
                lastStateChange = currentTime;
                animationFrame = 0;
                
                // Initialize bullet
                bulletX = 30;
                bulletY = 32;
                
                // Spawn smoke particles
                for (int i = 0; i < 5; i++) {
                    smoke[i].x = 30;
                    smoke[i].y = 32 + random(-3, 4);
                    smoke[i].life = 15 + random(0, 10);
                }
                
                Serial.println("Gun: BANG!");
            }
            break;
            
        case FIRING:
            if (currentTime - lastStateChange >= FIRING_TIME) {
                currentState = BULLET_FLYING;
                lastStateChange = currentTime;
                animationFrame = 0;
            }
            break;
            
        case BULLET_FLYING:
            // Move bullet
            bulletX += 4;
            
            if (currentTime - lastStateChange >= BULLET_TIME || bulletX >= 110) {
                currentState = IMPACT;
                lastStateChange = currentTime;
                animationFrame = 0;
                bulletX = 110;
            }
            break;
            
        case IMPACT:
            if (currentTime - lastStateChange >= IMPACT_TIME) {
                currentState = SMOKE_CLEAR;
                lastStateChange = currentTime;
                animationFrame = 0;
            }
            break;
            
        case SMOKE_CLEAR:
            if (currentTime - lastStateChange >= SMOKE_TIME) {
                currentState = READY;
                lastStateChange = currentTime;
                animationFrame = 0;
            }
            break;
    }
}

void displayGun() {
    updateGun();
    display.clearDisplay();
    
    // Draw title or status
    display.setTextSize(1);
    
    switch (currentState) {
        case READY: {
            display.setCursor(38, 2);
            display.print("READY...");
            
            // Gun in ready position
            drawPistol(20, 28, false);
            
            // Target on right
            drawTarget(110, 32);
            
            // Aiming line (dashed)
            for (int i = 35; i < 100; i += 5) {
                display.drawPixel(i, 32, WHITE);
            }
            
            // Bottom instruction
            display.setTextSize(1);
            display.setCursor(42, 56);
            display.print("FIRE!");
            break;
        }
        
        case FIRING: {
            display.setCursor(42, 2);
            display.print("BANG!");
            
            // Gun with recoil
            drawPistol(20, 28, true);
            
            // Muzzle flash
            drawMuzzleFlash(30, 32, animationFrame);
            
            // Target
            drawTarget(110, 32);
            
            // Shell casing ejecting
            int shellX = 15 - animationFrame * 2;
            int shellY = 24 + animationFrame * 3;
            display.drawRect(shellX, shellY, 2, 3, WHITE);
            break;
        }
        
        case BULLET_FLYING: {
            display.setCursor(30, 2);
            display.print("WHOOOOSH");
            
            // Gun returning from recoil
            drawPistol(20, 28, false);
            
            // Flying bullet
            drawBullet(bulletX, bulletY);
            
            // Target
            drawTarget(110, 32);
            
            // Draw active smoke particles
            for (int i = 0; i < 5; i++) {
                if (smoke[i].life > 0) {
                    int size = smoke[i].life / 5;
                    if (size > 0) {
                        display.drawCircle(smoke[i].x, smoke[i].y, size, WHITE);
                    }
                }
            }
            break;
        }
        
        case IMPACT: {
            display.setCursor(38, 2);
            display.print("IMPACT!");
            
            // Gun
            drawPistol(20, 28, false);
            
            // Impact explosion
            drawImpact(110, 32, animationFrame);
            break;
        }
        
        case SMOKE_CLEAR: {
            display.setCursor(26, 2);
            display.print("BULLSEYE!");
            
            // Gun
            drawPistol(20, 28, false);
            
            // Damaged target with hole
            drawTarget(110, 32);
            display.fillCircle(110, 32, 4, BLACK);
            display.fillCircle(110, 32, 2, WHITE);
            
            // Fading smoke at impact site
            int smokeSize = 10 - (animationFrame / 2);
            if (smokeSize > 0) {
                display.drawCircle(110, 32, smokeSize, WHITE);
                display.drawCircle(110, 32, smokeSize + 2, WHITE);
            }
            
            // Success stars
            if (animationFrame % 4 < 2) {
                display.drawPixel(100, 15, WHITE);
                display.drawPixel(120, 20, WHITE);
                display.drawPixel(105, 45, WHITE);
            }
            break;
        }
    }
    
    display.display();
}

