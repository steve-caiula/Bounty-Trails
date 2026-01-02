#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

// 1. Enumerations and Structures
// Define a GameState enum: MENU, GAMEPLAY, SETTINGS, etc.

typedef enum GameState
{
    STATE_START,
    STATE_MENU, 
    STATE_GAMEPLAY,
    STATE_GAMEOVER
} GameState;

// Define Player struct (position, speed, bounty, etc.)

typedef struct Player
{
    Vector2 position;
    float speed;
    int health;
    int dollars;
} Player;

int main (void)
{
    // 2. Global Variables
    // Current game state
    // Player instance

    GameState currentState = STATE_START;
    Player hunter = {0};
    hunter.health = 100;

    // 3. Initialization
    // Init window and audio
    // Set Target FPS

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bounty Trails");
    SetTargetFPS(60);

    // Setup initial values for our hunter
    hunter.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    hunter.speed = 300.0f;
    hunter.dollars = 0;
    
    // 4. Game Loop
    while (!WindowShouldClose()) {
        
        // 5. Update Logic (Decision making based on State)
        // switch(currentState) {
        //     case MENU:
        //         // Handle menu input (e.g., press ENTER to start)
        //         break;
        //     case GAMEPLAY:
        //         // Handle WASD movement
        //         // Handle mouse aiming
        //         // Handle shooting logic
        //         break;
        // }

        // 6. Rendering (Drawing based on State)
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // switch(currentState) {
            //     case MENU:
            //         // Draw title "BOUNTY TRAILS"
            //         // Draw "Press START to begin"
            //         break;
            //     case GAMEPLAY:
            //         // Draw environment (trail, dust, etc.)
            //         // Draw player (the hunter)
            //         // Draw targets and projectiles
            //         // Draw UI (wallet, current bounty)
            //         break;
            // }
        EndDrawing();
    }

    // 7. De-Initialization
    // Unload textures/sounds
    CloseWindow();

    return 0;
}