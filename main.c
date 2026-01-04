#include <stdbool.h>
#include <math.h>
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

    InitWindow (SCREEN_WIDTH, SCREEN_HEIGHT, "Bounty Trails");
    SetTargetFPS (60);

    Image icon = LoadImage ("resources/images/icon.png");
    SetWindowIcon (icon);
    UnloadImage (icon);

    Texture2D logoTexture = LoadTexture("resources/images/logo.png");

    Rectangle rect = {SCREEN_WIDTH/2,  SCREEN_HEIGHT/2, 150, 50};
    float lineThick = 5.0;

    // Setup initial values for our hunter
    hunter.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    hunter.speed = 300.0f;
    hunter.health = 100;
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

        switch (currentState)
        {
        case STATE_START:

            if (IsKeyPressed (KEY_ENTER))
            {
                currentState = STATE_MENU;
            }

            break;

        case STATE_MENU:
            
            break;

        case STATE_GAMEPLAY:
            
            if (hunter.health == 0)
            {
                currentState = STATE_GAMEOVER;
            }
            
            break;

        case STATE_GAMEOVER:

            if (IsKeyPressed (KEY_ENTER))
            {
                currentState = STATE_MENU;
                hunter.health = 100;
                hunter.dollars = 0;
                hunter.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
            }
            
            break;
        
        default:
            break;
        }

        // 6. Rendering (Drawing based on State)
        BeginDrawing ();
            
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
        
            switch (currentState)
            {
            case STATE_START:
                ClearBackground (BEIGE); 
                DrawTexture (logoTexture, SCREEN_WIDTH/2 - logoTexture.width/2, 30, WHITE);

                const char *startText = "Press ENTER to start";
                int fontSize = 20;
                int textWidth = MeasureText (startText, fontSize);
                float alpha = (sinf (GetTime () * 2.0f) + 1.0f) / 2.0f;

                DrawText (startText, SCREEN_WIDTH/2 - textWidth/2, 320, fontSize, Fade (BLACK, alpha));
                break;

            case STATE_MENU:
                ClearBackground (BEIGE);
                const char *newGameText = "NEW GAME";
                
                DrawRectangleLinesEx (rect, lineThick, DARKBROWN);
                DrawText (newGameText, SCREEN_WIDTH/2 - textWidth/2, 215, fontSize, DARKBROWN);
                break;

            case STATE_GAMEPLAY:
                ClearBackground (WHITE);
                break;

            case STATE_GAMEOVER:
                ClearBackground (BLACK);
                break;
        
            default:
                break;
            }

        EndDrawing ();
    }

    // 7. De-Initialization
    // Unload textures/sounds
    UnloadTexture(logoTexture);
    CloseWindow ();

    return 0;
}