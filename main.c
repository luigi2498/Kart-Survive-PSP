#include <QuickGame.h>  // QuickGame API.
#include <gu2gl.h>      // OpenGL header redefined.
#include <pspctrl.h>    // PSPSDK header for inputs.
#include <stdlib.h>
#include <time.h>

// Global variables.
QGSprite_t bg, base, gameover, pipe;    // Al the simple assets.
QGSprite_t score[10];   // Score sprites.
QGSprite_t kart[2];     // Main character sprite (3 images).

// Time variables.
QGTimer timer;

// Animation control variables.
int currentAnimation = 0;
float animationTime = 0.0f;

// Character data.
float kart_y, vel_y;

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
void draw();                    // Draw sprites.
void loadSprites();             // Load sprites.

// Main program.
int main(){
    if (QuickGame_Init() < 0){      // Failure state.
        return 1;
    }

    QuickGame_Graphics_Set2D();     // Set the Game's graphics.
    QuickGame_Timer_Start(&timer);  // Init the timer.

    srand(time(NULL));      

    loadSprites();
    resetGame();

    while (QuickGame_Running()){    // Game's main loop.
        update(QuickGame_Timer_Delta(&timer));  // Update current timer.
        draw();
    }

    QuickGame_Terminate();          // End state.
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

    for (int i = 0; i < 3; i++){
        pipes[i].x = 512 + 192 * i;         // X position plus an offset (out screen).
        pipes[i].y = 96 + rand() % 128;     // Y position between 96 and 128.
        pipes[i].active = true;
    }
}

void animationUpdate(){     // Character's state.
    kart[currentAnimation] -> transform.position.y = kart_y;    

    if (vel_y < -20) kart[currentAnimation] -> transform.rotation = -30.0f;

    else if (vel_y >= -20 && vel_y <= 20) kart[currentAnimation] -> transform.rotation = 0.0f;

    else if (vel_y > 20) kart[currentAnimation] -> transform.rotation = 30.0f;
}

void update(double dt){     // Update inputs (keys-buttons).
    QuickGame_Input_Update();   // Update input.

    accelerateAnim(dt);         // Update character animation.

    if (!dead){     // If is not dead...
        accelerateAnim(dt);     // Start sprite's animation.
        animationUpdate();      // Update animation in the render.

        // If X button is pressed (start the game).
        if (QuickGame_Button_Pressed(PSP_CTRL_CROSS)){
            started = true;
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
                    // Increase score.
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

void draw(){    // Render loop.
    QuickGame_Graphics_Start_Frame();   // Start frame.
    QuickGame_Graphics_Clear();         // Clear graphics.

    QuickGame_Sprite_Draw(bg);          // Draw backgroud.
    drawPipes();                        // Draw pipes.
    drawBaseScroll();                   // Background scroll animation.

    if (dead){      // If the character dies...
        QuickGame_Sprite_Draw(gameover);    // Game Over screen.
    }

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

    QGTexInfo kartIdle = {   // Mario small kart sprite (idle).
        .filename = "./assets/sprites/player/mario-small1.png",
        .flip = true,
        .vram = 0
    };
    kart[0] = QuickGame_Sprite_Create_Contained(120, 68, 68, 44, kartIdle);

    QGTexInfo kart1 = {   // Mario small kart sprite (accelerate 1).
        .filename = "./assets/sprites/player/mario-small2.png",
        .flip = true,
        .vram = 0
    };
    kart[1] = QuickGame_Sprite_Create_Contained(120, 68, 68, 44, kart1);

    QGTexInfo kart2 = {   // Mario small kart sprite (accelerate 2).
        .filename = "./assets/sprites/player/mario-small3.png",
        .flip = true,
        .vram = 0
    };
    kart[2] = QuickGame_Sprite_Create_Contained(120, 68, 68, 44, kart2);

    for (int i = 0; i < 3; i++){        // Kart's hitbox.
        kart[i] -> aabb_size.x = 16;
        kart[i] -> aabb_size.y = 16;
    }

    QGTexInfo gameoverTexInfo = {   // Mario small kart sprite (accelerate 2).
        .filename = "./assets/sprites/game-over.png",
        .flip = true,
        .vram = 0
    };
    gameover = QuickGame_Sprite_Create_Contained(240, 136, 256, 64, gameoverTexInfo);

    QGTexInfo pipeInfo = {
        .filename = "./assets/sprites/pipe.png",
        .flip = true,
        .vram = 0
    };
    pipe = QuickGame_Sprite_Create_Contained(0, 0, 64, 256, pipeInfo);
}