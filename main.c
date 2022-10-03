#include <QuickGame.h>

int main(){
    if (QuickGame_Init() < 0){  // Failure statement.
        return 1;
    }

    QuickGame_Graphics_Set2D();

    while (QuickGame_Running()){  // Game's main loop. 
        
    }

    QuickGame_Terminate();
    return 0;
}