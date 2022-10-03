#include <QuickGame.h>
#include <gu2gl.h>

// Global variables.
QGSprite_t bg, base, gameover, pipe;    // Al the simple assets.
QGSprite_t score[10];   // Score sprites.
QGSprite kar;           // Main character sprite.

QGTimer timer;          // Global timer.

// Functions and methods prototypes.
void update(double);
void drawBaseScroll();
void draw();
void loadSprites();

// Main program.
int main(){
    if (QuickGame_Init() < 0){  // Failure statement.
        return 1;
    }

    QuickGame_Graphics_Set2D();     // Set the Game's graphics.
    QuickGame_Timer_Start(&timer);  // Init the timer.

    loadSprites();

    while (QuickGame_Running()){  // Game's main loop. 
        update(QuickGame_Timer_Delta(&timer));  // Update current timer.
        draw();
    }

    QuickGame_Terminate();  // End statement.
    return 0;
}

// Functions and methods implementation.
void update(double dt){     // Update inputs (keys-buttons).
    QuickGame_Input_Update();
}

void drawBaseScroll(){      // Background scroll method.
    base -> transform.position.x = 128;
    QuickGame_Sprite_Draw(base);

    base -> transform.position.x = 384;
    QuickGame_Sprite_Draw(base);
}

void draw(){    // Render loop.
    QuickGame_Graphics_Start_Frame();   // Start frame.
    QuickGame_Graphics_Clear();         // Clear graphics.

    QuickGame_Sprite_Draw(bg);
    drawBaseScroll();

    QuickGame_Graphics_End_Frame(true);     // End frame.
}

void loadSprites(){     // Loading the sprites for the game.
    QGTexInfo bgTexInfo = {
        .filename = "./assets/sprites/bg.png",
        .flip = true,
        .vram = 0
    };
    bg = QuickGame_Sprite_Create_Contained(240, 192, 512, 512, bgTexInfo);

    QGTexInfo baseTexInfo = {
        .filename = "./assets/sprites/slide.png",
        .flip = true,
        .vram = 0
    };
    base = QuickGame_Sprite_Create_Contained(240, 16, 256, 64, baseTexInfo);
}