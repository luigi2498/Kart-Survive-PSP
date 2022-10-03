#include <QuickGame.h>

// Global variables.
QGSprite_t bg, base, gameover, pipe;    // Al the simple assets.
QGSprite_t score[10];   // Score sprites.
QGSprite kar;           // Main character sprite.

QGTimer timer;          // Global timer.

// Functions and methods prototypes.
void update(double);
void draw();
void loadSprites();

// Main program.
int main(){
    if (QuickGame_Init() < 0){  // Failure statement.
        return 1;
    }

    QuickGame_Graphics_Set2D();     // Set the Game's graphics.
    QuickGame_Timer_Start(&timer);  // Init the timer.

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

void draw(){    // Render loop.
    QuickGame_Graphics_Start_Frame();   // Start frame.
    QuickGame_Graphics_Clear();         // Clear graphics.
    QuickGame_Graphics_End_Frame();     // End frame.
}

void loadSprites(){

}