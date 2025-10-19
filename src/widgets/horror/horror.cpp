#include "horror.h"
#include "../../config/display.h"

// Horror scene states
enum HorrorScene {
    DARK_ALLEY,
    STALKER_APPEARS,
    KNIFE_GLINT,
    CHASE,
    SCREAM,
    BLOOD_SPLATTER,
    GAME_OVER
};

static HorrorScene currentScene = DARK_ALLEY;
static unsigned long lastSceneChange = 0;
static unsigned long lastAnimFrame = 0;
static int animationFrame = 0;

// Positions
static int victimX = 10;
static int stalkerX = -20;

// Blood particles
struct BloodDrop {
    int x, y;
    int life;
};
static BloodDrop blood[8];

// Timing constants
const unsigned long ALLEY_TIME = 1200;
const unsigned long STALKER_TIME = 1500;
const unsigned long KNIFE_TIME = 800;
const unsigned long CHASE_TIME = 1500;
const unsigned long SCREAM_TIME = 600;
const unsigned long BLOOD_TIME = 1000;
const unsigned long GAMEOVER_TIME = 1500;
const unsigned long ANIM_FRAME_TIME = 60;

void initHorror() {
    currentScene = DARK_ALLEY;
    lastSceneChange = millis();
    animationFrame = 0;
    victimX = 10;
    stalkerX = -20;
    
    // Initialize blood particles
    for (int i = 0; i < 8; i++) {
        blood[i].life = 0;
    }
}

// Draw a person silhouette
void drawPerson(int x, int y, bool running) {
    // Head
    display.fillCircle(x, y - 8, 3, WHITE);
    
    // Body
    display.drawLine(x, y - 5, x, y + 4, WHITE);
    
    if (running) {
        // Running pose - arms and legs in motion
        int legSwing = (animationFrame % 2) ? 4 : -4;
        display.drawLine(x, y - 3, x - 4, y - 3 + legSwing, WHITE); // Left arm
        display.drawLine(x, y - 3, x + 4, y - 3 - legSwing, WHITE); // Right arm
        display.drawLine(x, y + 4, x - 3, y + 10 + legSwing, WHITE); // Left leg
        display.drawLine(x, y + 4, x + 3, y + 10 - legSwing, WHITE); // Right leg
    } else {
        // Walking pose
        display.drawLine(x, y - 3, x - 3, y + 2, WHITE); // Left arm
        display.drawLine(x, y - 3, x + 3, y + 2, WHITE); // Right arm
        display.drawLine(x, y + 4, x - 2, y + 10, WHITE); // Left leg
        display.drawLine(x, y + 4, x + 2, y + 10, WHITE); // Right leg
    }
}

// Draw stalker with weapon
void drawStalker(int x, int y, bool showKnife) {
    // Head (larger, more menacing)
    display.fillCircle(x, y - 8, 4, WHITE);
    
    // Eyes (hollow/scary)
    display.drawPixel(x - 2, y - 9, BLACK);
    display.drawPixel(x + 2, y - 9, BLACK);
    
    // Body
    display.drawLine(x, y - 4, x, y + 4, WHITE);
    display.drawLine(x - 1, y - 4, x - 1, y + 4, WHITE); // Thicker
    
    // Arms
    display.drawLine(x, y - 2, x - 5, y + 3, WHITE);
    
    if (showKnife) {
        // Knife in hand
        display.drawLine(x + 1, y - 2, x + 8, y - 5, WHITE);
        display.fillTriangle(x + 8, y - 5, x + 12, y - 6, x + 11, y - 3, WHITE);
    } else {
        display.drawLine(x, y - 2, x + 5, y + 3, WHITE);
    }
    
    // Legs
    display.drawLine(x, y + 4, x - 3, y + 10, WHITE);
    display.drawLine(x, y + 4, x + 3, y + 10, WHITE);
}

// Draw dark alley background
void drawAlley() {
    // Buildings/walls
    display.drawLine(0, 50, 30, 20, WHITE);
    display.drawLine(30, 20, 30, 0, WHITE);
    display.drawLine(98, 20, 128, 50, WHITE);
    display.drawLine(98, 20, 98, 0, WHITE);
    
    // Ground
    display.drawLine(0, 50, 128, 50, WHITE);
    
    // Some windows (dark rectangles)
    display.drawRect(10, 10, 4, 4, WHITE);
    display.drawRect(18, 15, 4, 4, WHITE);
    display.drawRect(105, 10, 4, 4, WHITE);
    display.drawRect(113, 15, 4, 4, WHITE);
}

// Draw knife glint effect
void drawKnifeGlint(int x, int y, int frame) {
    // Knife blade
    display.fillTriangle(x, y, x + 8, y - 2, x + 7, y + 2, WHITE);
    
    // Glint sparkle
    if (frame % 2 == 0) {
        display.drawPixel(x + 4, y - 2, WHITE);
        display.drawPixel(x + 5, y - 1, WHITE);
        display.drawPixel(x + 6, y, WHITE);
        display.drawPixel(x + 5, y + 1, WHITE);
        display.drawPixel(x + 4, y + 2, WHITE);
    }
}

void updateHorror() {
    unsigned long currentTime = millis();
    
    // Update animation frame
    if (currentTime - lastAnimFrame >= ANIM_FRAME_TIME) {
        animationFrame++;
        lastAnimFrame = currentTime;
        
        // Update blood particles
        for (int i = 0; i < 8; i++) {
            if (blood[i].life > 0) {
                blood[i].y += 2;
                blood[i].x += random(-1, 2);
                blood[i].life--;
            }
        }
    }
    
    // State machine
    switch (currentScene) {
        case DARK_ALLEY:
            victimX += 1;
            if (currentTime - lastSceneChange >= ALLEY_TIME) {
                currentScene = STALKER_APPEARS;
                lastSceneChange = currentTime;
                animationFrame = 0;
                stalkerX = 100;
                Serial.println("Horror: Stalker appears...");
            }
            break;
            
        case STALKER_APPEARS:
            if (currentTime - lastSceneChange >= STALKER_TIME) {
                currentScene = KNIFE_GLINT;
                lastSceneChange = currentTime;
                animationFrame = 0;
                Serial.println("Horror: Knife revealed!");
            }
            break;
            
        case KNIFE_GLINT:
            if (currentTime - lastSceneChange >= KNIFE_TIME) {
                currentScene = CHASE;
                lastSceneChange = currentTime;
                animationFrame = 0;
                Serial.println("Horror: Chase begins!");
            }
            break;
            
        case CHASE:
            victimX += 2;
            stalkerX += 2;
            if (currentTime - lastSceneChange >= CHASE_TIME) {
                currentScene = SCREAM;
                lastSceneChange = currentTime;
                animationFrame = 0;
                
                // Spawn blood particles
                for (int i = 0; i < 8; i++) {
                    blood[i].x = 64 + random(-10, 10);
                    blood[i].y = 25 + random(-5, 5);
                    blood[i].life = 15 + random(0, 10);
                }
                Serial.println("Horror: SCREAM!");
            }
            break;
            
        case SCREAM:
            if (currentTime - lastSceneChange >= SCREAM_TIME) {
                currentScene = BLOOD_SPLATTER;
                lastSceneChange = currentTime;
                animationFrame = 0;
            }
            break;
            
        case BLOOD_SPLATTER:
            if (currentTime - lastSceneChange >= BLOOD_TIME) {
                currentScene = GAME_OVER;
                lastSceneChange = currentTime;
                animationFrame = 0;
            }
            break;
            
        case GAME_OVER:
            if (currentTime - lastSceneChange >= GAMEOVER_TIME) {
                // Reset
                currentScene = DARK_ALLEY;
                lastSceneChange = currentTime;
                animationFrame = 0;
                victimX = 10;
                stalkerX = -20;
            }
            break;
    }
}

void displayHorror() {
    updateHorror();
    display.clearDisplay();
    
    switch (currentScene) {
        case DARK_ALLEY: {
            // Dark alley atmosphere
            drawAlley();
            
            // Victim walking alone
            drawPerson(victimX, 40, false);
            
            // Ominous text
            display.setTextSize(1);
            display.setCursor(20, 2);
            display.print("Late Night...");
            
            // Moon
            display.drawCircle(110, 10, 5, WHITE);
            display.fillCircle(108, 10, 4, BLACK);
            break;
        }
        
        case STALKER_APPEARS: {
            drawAlley();
            
            // Victim still walking
            drawPerson(60, 40, false);
            
            // Stalker emerges from shadows
            drawStalker(stalkerX, 40, false);
            
            // Shadow effect - flicker
            if (animationFrame % 3 == 0) {
                display.fillTriangle(stalkerX - 5, 50, stalkerX + 8, 50, stalkerX, 35, BLACK);
            }
            
            // Warning text
            display.setTextSize(1);
            display.setCursor(30, 2);
            display.print("DANGER...");
            
            // Tension dots
            for (int i = 0; i < 3; i++) {
                if (animationFrame % 6 > i) {
                    display.drawPixel(100 + i * 3, 5, WHITE);
                }
            }
            break;
        }
        
        case KNIFE_GLINT: {
            drawAlley();
            
            // Victim unaware
            drawPerson(65, 40, false);
            
            // Stalker with knife
            drawStalker(85, 40, true);
            
            // Knife glint effect
            drawKnifeGlint(95, 35, animationFrame);
            
            // Dramatic text
            display.setTextSize(1);
            display.setCursor(10, 2);
            display.print("*GLINT*");
            
            // Screen flicker effect
            if (animationFrame % 4 == 0) {
                for (int i = 0; i < 20; i++) {
                    display.drawPixel(random(128), random(20), WHITE);
                }
            }
            break;
        }
        
        case CHASE: {
            // Minimal background - speed effect
            display.drawLine(0, 50, 128, 50, WHITE);
            
            // Victim running
            drawPerson(victimX % 128, 40, true);
            
            // Stalker chasing
            drawStalker((stalkerX - 15) % 128, 40, true);
            
            // Speed lines
            for (int i = 0; i < 5; i++) {
                int lineX = (animationFrame * 10 + i * 25) % 128;
                display.drawLine(lineX, 30, lineX - 15, 35, WHITE);
                display.drawLine(lineX, 45, lineX - 15, 40, WHITE);
            }
            
            // Panic text
            display.setTextSize(2);
            display.setCursor(30, 2);
            display.print("RUN!");
            break;
        }
        
        case SCREAM: {
            // Dark screen
            display.fillRect(0, 0, 128, 64, WHITE);
            display.fillRect(2, 2, 124, 60, BLACK);
            
            // SCREAM text
            display.setTextSize(3);
            display.setCursor(10, 20);
            display.print("AHHH!");
            
            // Blood drops falling
            for (int i = 0; i < 8; i++) {
                if (blood[i].life > 0) {
                    display.fillCircle(blood[i].x, blood[i].y, 2, WHITE);
                }
            }
            break;
        }
        
        case BLOOD_SPLATTER: {
            // Blood splatter effect
            display.fillRect(0, 0, 128, 64, BLACK);
            
            // Random blood splatters
            for (int i = 0; i < 30; i++) {
                int x = 64 + random(-40, 40);
                int y = 32 + random(-25, 25);
                int size = random(1, 4);
                display.fillCircle(x, y, size, WHITE);
            }
            
            // Dripping blood
            for (int i = 0; i < 8; i++) {
                if (blood[i].life > 0) {
                    display.fillCircle(blood[i].x, blood[i].y, 2, WHITE);
                    display.drawLine(blood[i].x, blood[i].y - 5, blood[i].x, blood[i].y, WHITE);
                }
            }
            
            // Dramatic text
            display.setTextSize(1);
            display.setCursor(40, 2);
            display.print("NO...");
            break;
        }
        
        case GAME_OVER: {
            // Dark red screen effect
            if (animationFrame % 2 == 0) {
                display.fillRect(0, 0, 128, 64, WHITE);
                display.fillRect(3, 3, 122, 58, BLACK);
            }
            
            // Game over text
            display.setTextSize(2);
            display.setCursor(20, 15);
            display.print("VICTIM");
            
            display.setTextSize(1);
            display.setCursor(25, 35);
            display.print("REST IN PEACE");
            
            // Tombstone
            display.drawRect(52, 45, 24, 15, WHITE);
            display.drawTriangle(52, 45, 64, 40, 76, 45, WHITE);
            display.setCursor(58, 52);
            display.setTextSize(1);
            display.print("RIP");
            
            // Flickering candles
            if (animationFrame % 3 != 0) {
                display.drawLine(45, 55, 45, 50, WHITE);
                display.drawPixel(45, 49, WHITE);
                display.drawLine(83, 55, 83, 50, WHITE);
                display.drawPixel(83, 49, WHITE);
            }
            break;
        }
    }
    
    display.display();
}

