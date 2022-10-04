#include <QuickGame.h>
#include <gu2gl.h>

// Global variables.
QGSprite_t bg, base, gameover, pipe;    // Al the simple assets.
QGSprite_t score[10];   // Score sprites.
QGSprite kart[3];       // Main character sprite.

// Time variables.
QGTimer timer;          // Global timer.

// Animation control variables.
int currentAnimation = 0;
float animationTime = 0.0f;

// Functions and methods prototypes.
void accelerateAnim(double);    // Animate the sprite.
void update(double);            // For inputs.
void drawBaseScroll();          // For background.
void draw();                    // Draw sprites.
void loadSprites();             // Load sprites.

// Main program.
int main(){
    if (QuickGame_Init() < 0){      // Failure statement.
        return 1;
    }

    QuickGame_Graphics_Set2D();     // Set the Game's graphics.
    QuickGame_Timer_Start(&timer);  // Init the timer.

    loadSprites();

    while (QuickGame_Running()){    // Game's main loop.
        update(QuickGame_Timer_Delta(&timer));  // Update current timer.
        draw();
    }

    QuickGame_Terminate();          // End statement.
    return 0;
}

// Functions and methods implementation.
void accelerateAnim(double dt){
    animationTime += dt;

    if (animationTime > 0.125f){    // 0.125f = 8 times per second.
        currentAnimation++;
        animationTime = 0.0f;

        if (currentAnimation == 3) currentAnimation = 0;    // Restart animation.
    }
}

void update(double dt){     // Update inputs (keys-buttons).
    QuickGame_Input_Update();

    accelerateAnim(dt);
}

void drawBaseScroll(){      // Background scroll method.
    glTexOffset(timer.total * 0.5f, 0.0f);  // Init scroll (left).

    base -> transform.position.x = 128;     // Update x position.
    QuickGame_Sprite_Draw(base);            // Draw the sprite.

    base -> transform.position.x = 384;     // Update x position.
    QuickGame_Sprite_Draw(base);            // Draw sprite.
    glTexOffset(0.0f, 0.0f);                // Reset scroll.
}

void draw(){    // Render loop.
    QuickGame_Graphics_Start_Frame();   // Start frame.
    QuickGame_Graphics_Clear();         // Clear graphics.

    QuickGame_Sprite_Draw(bg);          // Draw backgroud.
    drawBaseScroll();                   // Background scroll animation.

    QuickGame_Sprite_Draw(kart[currentAnimation]);  // Draw main character.

    QuickGame_Graphics_End_Frame(true);     // End frame.
}

void loadSprites(){     // Loading the sprites for the game.
    QGTexInfo bgTexInfo = {     // Backgroud sprite.
        .filename = "./assets/sprites/bg.png",
        .flip = true,
        .vram = 0
    };
    bg = QuickGame_Sprite_Create_Contained(240, 192, 512, 512, bgTexInfo);

    QGTexInfo baseTexInfo = {   // Floor sprite.
        .filename = "./assets/sprites/slide.png",
        .flip = true,
        .vram = 0
    };
    base = QuickGame_Sprite_Create_Contained(240, 16, 256, 64, baseTexInfo);

    QGTexInfo kartTexInfo = {   // Mario small kart sprite.
        .filename = "./assets/sprites/player/mario-small.png",
        .flip = true,
        .vram = 0
    };
    kart = QuickGame_Sprite_Create_Contained(160, 16, 34, 22, kartTexInfo);
}