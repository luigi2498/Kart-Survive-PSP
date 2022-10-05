#include <QuickGame.h>  // QuickGame API.
#include <gu2gl.h>      // OpenGL header redefined.
#include <pspctrl.h>    // PSPSDK header for inputs.
#include <stdlib.h>     // Import the rand() function.
#include <time.h>       // Utillities for the random generation.
#include <stdio.h>      // Input output functions.

// Global variables.
// Texture variables.
QGSprite_t bg, base, gameover, pipe;    // Al the simple assets.
QGSprite_t score[10];   // Score sprites.
QGSprite_t kart[3];     // Main character sprite (3 images + buffer).

// Time variables.
QGTimer timer;

// Animation control variables.
int currentAnimation = 0;
float animationTime = 0.0f;

// Character data.
float kart_y, vel_y;

// Score.
int currentScore;

// Game state.
bool started, dead;

// Structs.
// Pipe struct/class.
typedef struct{
    float x, y;
    bool active;
} PipeInfo;

PipeInfo pipes[3];  // Pipe's (struct/class) instance.

// Functions and methods prototypes.
void accelerateAnim(double);    // Animate the sprite.
void resetGame();               // Game control.
void update(double);            // For inputs.
void drawPipes();               // For obstacles.
void drawBaseScroll();          // For background.
void drawScore();               // For score.
void draw();                    // Draw sprites.
void loadSprites();             // Load sprites.

// Main program.
int main(){
    if (QuickGame_Init() < 0){      // Failure state.
        return 1;
    }

    QuickGame_Graphics_Set2D();     // Set the Game graphics.
    QuickGame_Timer_Start(&timer);  // Init the timer.

    srand(time(NULL));      // Just for the rand() function.

    loadSprites();          // Load all the game sprites.
    resetGame();            // Reset condition.

    while (QuickGame_Running()){    // Game's main loop.
        update(QuickGame_Timer_Delta(&timer));  // Update current timer.
        draw();     // Render.
    }

    QuickGame_Terminate();  // End state.
    return 0;
}

// Functions and methods implementation.
void accelerateAnim(double dt){     // Animates the main sprite.
    animationTime += dt;

    if (animationTime > 0.125f){    // 0.125f = 8 times per second.
        currentAnimation++;         // Increment de image value.
        animationTime = 0.0f;       // Reset animarion timer.

        if (currentAnimation == 3) currentAnimation = 0;    // Restart animation.
    }
}

void resetGame(){       // Reset to default values.
    started = false;    // Condition.
    dead = false;       // Condition.
    kart_y = 68.0f;     // Initial position.
    vel_y = 0.0f;       // Reset vertical speed.
    currentScore = 0;   // Initial score.

    for (int i = 0; i < 3; i++){
        pipes[i].x = 512 + 192 * i;         // X position plus an offset (out screen).
        pipes[i].y = 96 + rand() % 128;     // Y position between 96 and 128.
        pipes[i].active = true;             // Instance a pipe.
    }
}

void animationUpdate(){     // Character's state.
    kart[currentAnimation] -> transform.position.y = kart_y;    // Jump.

    if (vel_y < -20) kart[currentAnimation] -> transform.rotation = -30.0f;     // Down rotation.

    else if (vel_y >= -20 && vel_y <= 20) kart[currentAnimation] -> transform.rotation = 0.0f;  // Reset rotation.

    else if (vel_y > 20) kart[currentAnimation] -> transform.rotation = 30.0f;  // Up rotation.
}

void update(double dt){     // Update inputs (keys-buttons).
    QuickGame_Input_Update();   // Update input.

    accelerateAnim(dt);         // Update character animation.

    if (!dead){     // If is not dead...
        accelerateAnim(dt);     // Start sprite's animation.
        animationUpdate();      // Update animation in the render.

        // If X button is pressed (start the game).
        if (QuickGame_Button_Pressed(PSP_CTRL_CROSS)){
            started = true;     // Game started.
            vel_y = 192;        // Jump.
        }

        if (started){   // If the game is started...
            vel_y -= 512.0f * dt;   // Decrement speed.
            kart_y += vel_y * dt;   // Acts like gravity.

            if (kart_y < 48){   // If the kart colides with the floor...
                dead = true;    // It dies.
            }

            for(int i = 0; i < 3; i++){     // Pipes.
                pipes[i].x -= 128.0f * dt;  // Move 128 pixels per second.

                // Pipes passed.
                if (pipes[i].x < 160 && pipes[i].active){
                    pipes[i].active = false;    // Deactivate it.
                    currentScore++;             // Increase score.
                }

                // Pipes respawning.
                if (pipes[i].x < -32){
                    pipes[i].x += 576;
                    pipes[i].y = 96 + rand() % 128;
                    pipes[i].active = true;
                }
            }
        }
    }

    else{
        currentAnimation = 0;           // Stop kart animation.
        QuickGame_Timer_Reset(&timer);  // Reset timer.

        if (QuickGame_Button_Pressed(PSP_CTRL_CROSS)){  // Reset the game if X is pressed.
            resetGame();
        }
    }
}

void drawPipes(){
    for(int i = 0; i < 3; i++){
        pipe -> transform.position.x = pipes[i].x;
        pipe -> transform.position.y = pipes[i].y - 128 - 60;   // Default 40.
        pipe -> transform.rotation = 0.0f;
        QuickGame_Sprite_Draw(pipe);

        if (QuickGame_Sprite_Intersects(pipe, kart[currentAnimation])){
            dead = true;
        }

        pipe -> transform.position.x = pipes[i].x;
        pipe -> transform.position.y = pipes[i].y + 128 + 60;   // Default 40.
        pipe -> transform.rotation = 180.0f;
        QuickGame_Sprite_Draw(pipe);

        if (QuickGame_Sprite_Intersects(pipe, kart[currentAnimation])){
            dead = true;
        }
    }
}

void drawBaseScroll(){      // Background scroll method.
    glTexOffset(timer.total * 0.5f, 0.0f);  // Init scroll (left).

    base -> transform.position.x = 128;     // Update x position.
    QuickGame_Sprite_Draw(base);            // Draw the sprite.

    base -> transform.position.x = 384;     // Update x position.
    QuickGame_Sprite_Draw(base);            // Draw sprite.
    glTexOffset(0.0f, 0.0f);                // Reset scroll.
}

void drawScore(){           // Draw score.
    int sc = currentScore;  // Score.
    int digits = 0;         // Quantity of digits.
    float xOffset, xn;      // x offset and x current position.

    while(sc > 0){      // For all digits.
        sc /= 10;       // Ten's multiple
        digits++;       // Increase digits.
    }

    xOffset = -((float)digits - 1) / 2.0f;     // x offset for the score position.
    xOffset *= 12.0f;   // Spaced.

    xn = 0.0f;          // Current position.

    sc = currentScore;  // Update score auxiliar variable.
    
    while(sc > 0){          // For all digits.
        int c = sc % 10;    // Score module 10.
        sc /= 10;           // Ten's multiple

        score[c] -> transform.position.x = -xOffset + 240 - xn;     // Move on x axis.
        score[c] -> transform.position.y = 192;                     // Move on y axis.

        xn += 24.0f;    // Update x position.

        QuickGame_Sprite_Draw(score[c]);    // Draw sprite.
    }

}

void draw(){    // Render loop.
    QuickGame_Graphics_Start_Frame();   // Start frame.
    QuickGame_Graphics_Clear();         // Clear graphics.

    QuickGame_Sprite_Draw(bg);          // Draw backgroud.
    drawPipes();                        // Draw pipes.
    drawBaseScroll();                   // Background scroll animation.

    QuickGame_Sprite_Draw(kart[currentAnimation]);  // Draw main character.

    // If the character dies...
    if (dead) QuickGame_Sprite_Draw(gameover);    // Display game over screen.

    drawScore();        // Draw score on screen.

    QuickGame_Graphics_End_Frame(true);     // End frame.
}

void loadSprites(){     // Loading the sprites for the game.
    QGTexInfo bgTexInfo = {     // Backgroud sprite.
        .filename = "./assets/sprites/background/bg2.png",
        .flip = true,
        .vram = 0
    };
    bg = QuickGame_Sprite_Create_Contained(240, 192, 512, 512, bgTexInfo);

    QGTexInfo baseTexInfo = {   // Floor sprite.
        .filename = "./assets/sprites/misc/slide.png",
        .flip = true,
        .vram = 0
    };
    base = QuickGame_Sprite_Create_Contained(240, 16, 256, 64, baseTexInfo);

    QGTexInfo kartIdle = {   // Small kart sprite (idle).
        .filename = "./assets/sprites/player/luigi-small1.png",
        .flip = true,
        .vram = 0
    };
    kart[0] = QuickGame_Sprite_Create_Contained(120, 68, 68, 44, kartIdle);

    QGTexInfo kart1 = {     // Small kart sprite (accelerate 1).
        .filename = "./assets/sprites/player/luigi-small1.png",
        .flip = true,
        .vram = 0
    };
    kart[1] = QuickGame_Sprite_Create_Contained(120, 68, 68, 44, kart1);

    QGTexInfo kart2 = {     // Small kart sprite (accelerate 2).
        .filename = "./assets/sprites/player/luigi-small1.png",
        .flip = true,
        .vram = 0
    };
    kart[2] = QuickGame_Sprite_Create_Contained(120, 68, 68, 44, kart2);

    for (int i = 0; i < 3; i++){     // Kart's hitbox.
        kart[i] -> aabb_size.x = 16;
        kart[i] -> aabb_size.y = 16;
    }

    QGTexInfo gameoverTexInfo = {   // Game over logo.
        .filename = "./assets/sprites/misc/game-over.png",
        .flip = true,
        .vram = 0
    };
    gameover = QuickGame_Sprite_Create_Contained(240, 136, 256, 64, gameoverTexInfo);

    QGTexInfo pipeInfo = {          // Pipe sprite.
        .filename = "./assets/sprites/misc/pipe.png",
        .flip = true,
        .vram = 0
    };
    pipe = QuickGame_Sprite_Create_Contained(0, 0, 64, 256, pipeInfo);

    for (int i = 0; i < 10; i++){
        char filename[256];     // Buffer for all the files names.
        sprintf(filename, "./assets/sprites/count/%d.png", i);

        QGTexInfo scoreInfo = {     // Score sprites.
            .filename = filename,
            .flip = true,
            .vram = 0
        };
        score[i] = QuickGame_Sprite_Create_Contained(240, 136, 32, 64, scoreInfo);
    }
}