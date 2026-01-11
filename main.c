#include <stdbool.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define PLAYER_WIDTH 35
#define PLAYER_HEIGHT 40
#define BULLET_RADIUS 5

// 1. Enumerations and Structures
// Define a GameState enum: MENU, GAMEPLAY, SETTINGS, etc.

typedef enum
{
    STATE_START,
    STATE_MENU, 
    STATE_GAMEPLAY,
    STATE_GAMEOVER
} GameState;

// Define Player struct (position, speed, bounty, etc.)

typedef struct
{
    Vector2 position;
    Vector2 direction;
    float speed;
    int health;
    int dollars;
    Rectangle rect;
} Player;

typedef struct 
{
    Vector2 position;
    Vector2 direction;
    bool active;
    float speed;
    float radius;
    int damage;
} Bullet;

typedef struct
{
    Rectangle rect;
    const char *text;
    int fontSize;
    int textWidth;
    bool isHovered;
    Color buttonColor;
} MenuButton;


int main (void)
{
    // 2. Current game state

    GameState currentState = STATE_START;

    // 3. Initialization
    // Init window and audio
    // Set Target FPS

    InitWindow (SCREEN_WIDTH, SCREEN_HEIGHT, "Bounty Trails");
    SetTargetFPS (60);

    Image icon = LoadImage ("resources/images/icon.png");
    SetWindowIcon (icon);
    UnloadImage (icon);

    Texture2D logoTexture = LoadTexture ("resources/images/logo.png");

    // Setup values for new game button
    MenuButton newGame;
    newGame.rect = (Rectangle){SCREEN_WIDTH/2.0f - 75, SCREEN_HEIGHT/2.0f - 25, 150, 50};
    newGame.text = "NEW GAME";
    newGame.fontSize  = 20;
    newGame.textWidth = MeasureText (newGame.text, newGame.fontSize);
    newGame.isHovered = false;
    newGame.buttonColor = DARKBROWN;
   
    // Setup initial values for player
    Player player;
    player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    player.speed = 300.0f;
    player.health = 100;
    player.dollars = 0;
    player.rect = (Rectangle){ SCREEN_WIDTH/2, SCREEN_HEIGHT/2, PLAYER_WIDTH, PLAYER_HEIGHT };
    player.rect.x = player.position.x;
    player.rect.y = player.position.y;

    // Setup initial values for bullets
    Bullet bullet;
    bullet.position;
    bullet.direction;
    bullet.active = false;
    bullet.speed = 500.0f;
    bullet.radius = BULLET_RADIUS;
    bullet.damage = 100;

    // Vector 2 position bounds
    Vector2 minBounds = { 0, 0 };
    Vector2 maxBounds = { SCREEN_WIDTH - PLAYER_WIDTH, SCREEN_HEIGHT - PLAYER_HEIGHT };
    
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
            newGame.isHovered = CheckCollisionPointRec (GetMousePosition (), newGame.rect);
            newGame.buttonColor = (newGame.isHovered) ? MAROON : DARKBROWN;

            if (newGame.isHovered && IsMouseButtonPressed (MOUSE_LEFT_BUTTON)) 
            {
                currentState = STATE_GAMEPLAY;
            }
            
            break;

        case STATE_GAMEPLAY:
            
            // Player centre, useful for managing aim e bullet shooting logic
            Vector2 playerCenter = 
            { 
                player.position.x + PLAYER_WIDTH/2.0f, 
                player.position.y + PLAYER_HEIGHT/2.0f 
            };

            // Shoot
            if (IsMouseButtonPressed (MOUSE_LEFT_BUTTON) && !bullet.active) 
            {
                bullet.active = true;
                bullet.position = playerCenter;
    
                // Direction: Mouse - Player
                Vector2 target = GetMousePosition ();
                Vector2 diff = Vector2Subtract (target, playerCenter);
    
                // Vector long 1.0 for constant speed
                bullet.direction = Vector2Normalize (diff);
            }

            if (bullet.active) 
            {
                bullet.position.x += bullet.direction.x * bullet.speed * GetFrameTime();
                bullet.position.y += bullet.direction.y * bullet.speed * GetFrameTime();

                // Disable bullet when it leaves the screen
                if (bullet.position.x < 0 || bullet.position.x > SCREEN_WIDTH ||
                bullet.position.y < 0 || bullet.position.y > SCREEN_HEIGHT) 
                {
                    bullet.active = false;
                }
            }
        
            // Player death
            if (player.health == 0)
            {
                currentState = STATE_GAMEOVER;
            }

            // Player movement
            if (IsKeyDown (KEY_W))
            {
                player.position.y -= player.speed * GetFrameTime ();
            }
            
            if (IsKeyDown (KEY_S))
            {
                player.position.y += player.speed * GetFrameTime ();
            }
            
            if (IsKeyDown (KEY_A))
            {
                player.position.x -= player.speed * GetFrameTime ();
            }

            if (IsKeyDown (KEY_D))
            {
                player.position.x += player.speed * GetFrameTime ();
            }
            
            // Keep player inside the bounds
            player.position = Vector2Clamp (player.position, minBounds, maxBounds);

            // Player location on screen
            player.rect.x = player.position.x;
            player.rect.y = player.position.y;
            
            break;

        case STATE_GAMEOVER:

            if (IsKeyPressed (KEY_ENTER))
            {
                currentState = STATE_MENU;
                player.health = 100;
                player.dollars = 0;
                player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
            }
            
            break;
        
        default:
            break;
        }

        // Rendering (Drawing based on State)
        BeginDrawing ();
            
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
                
                DrawRectangleRec (newGame.rect, newGame.buttonColor);

                float textX = newGame.rect.x + (newGame.rect.width / 2.0f - newGame.textWidth / 2.0f);
                float textY = newGame.rect.y + (newGame.rect.height / 2.0f - newGame.fontSize / 2.0f);
                DrawText (newGame.text, textX, textY, newGame.fontSize, BEIGE);
                break;

            case STATE_GAMEPLAY:
                ClearBackground (WHITE);

                DrawRectangleRec (player.rect, RED);
                DrawCircleV (bullet.position, bullet.radius, BLACK);
                break;

            case STATE_GAMEOVER:
                ClearBackground (BLACK);
                break;
        
            default:
                break;
            }

        EndDrawing ();
    }

    // De-Initialization
    // Unload textures/sounds
    UnloadTexture(logoTexture);
    CloseWindow ();

    return 0;
}