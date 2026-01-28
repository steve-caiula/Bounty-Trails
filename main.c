#include <stdbool.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"

#define PLAYER_WIDTH 35
#define PLAYER_HEIGHT 40
#define ENEMY_WIDTH 35
#define ENEMY_HEIGHT 40
#define BULLET_RADIUS 5
#define MAX_BULLETS 50

// 1. Enumerations and Structures
// Define a GameState enum: MENU, GAMEPLAY, SETTINGS, etc.

typedef enum
{
    STATE_START,
    STATE_MENU, 
    STATE_GAMEPLAY,
    STATE_GAMEOVER
} GameState;

// Define Player struct
typedef struct Player
{
    Vector2 position;
    Vector2 direction;
    float speed;
    int health;
    int maxHealth;
    int dollars;
    Rectangle rect;
} Player;

typedef struct PlayerHud
{
    Rectangle healthBar;
    Rectangle backgroundBar;
    float maxHealth;
    
    Vector2 dollarsPosition;
    int fontSize;
    Color moneyColor;
} PlayerHud;

typedef struct Bullet
{
    Vector2 position;
    Vector2 direction;
    bool active;
    float speed;
    float radius;
    int damage;
} Bullet;

typedef struct Enemy
{
    Vector2 position;
    Vector2 direction;
    bool active;
    float speed;
    int health;
    int bounty;
    Rectangle rect;
} Enemy;


typedef struct MenuButton
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

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow (0, 0, "Bounty Trails");
    ToggleFullscreen();
    SetTargetFPS (60);
    float screenWidth = GetScreenWidth ();
    float screenHeight = GetScreenHeight ();

    Image icon = LoadImage ("resources/images/icon.png");
    SetWindowIcon (icon);
    UnloadImage (icon);

    Texture2D logoTexture = LoadTexture ("resources/images/logo.png");

    // Setup values for new game button
    MenuButton newGame;
    newGame.rect.width = 300.0f;  
    newGame.rect.height = 80.0f; 
    newGame.rect.x = screenWidth / 2.0f - newGame.rect.width / 2.0f;
    newGame.rect.y = screenHeight / 2.0f - newGame.rect.height / 2.0f;
    newGame.text = "NEW GAME";
    newGame.fontSize  = 40;
    newGame.textWidth = MeasureText (newGame.text, newGame.fontSize);
    newGame.isHovered = false;
    newGame.buttonColor = DARKBROWN;
   
    // Setup initial values for player
    Player player;
    player.position = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    player.speed = 300.0f;
    player.health = 100;
    player.maxHealth = 100;
    player.dollars = 0;
    player.rect = (Rectangle){ player.position.x, player.position.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    player.rect.x = player.position.x;
    player.rect.y = player.position.y;

    // Setup initial values for player's HUD
    PlayerHud playerHUD;
    playerHUD.healthBar.width = 200.0f;
    playerHUD.healthBar.height = 50.0f;
    playerHUD.healthBar.x = 20.0f;
    playerHUD.healthBar.y = 20.0f;
    playerHUD.backgroundBar.width = 200.0f;
    playerHUD.backgroundBar.height = 50.0f;
    playerHUD.backgroundBar.x = 20.0f;
    playerHUD.backgroundBar.y = 20.0f;
    playerHUD.maxHealth = player.maxHealth;
    playerHUD.dollarsPosition = (Vector2){ 20.0f, 80.0f};
    playerHUD.fontSize = 40;
    playerHUD.moneyColor = DARKGREEN;

    // Setup initial values for bullets
    Bullet bullet[MAX_BULLETS];
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullet[i].position;
        bullet[i].direction;
        bullet[i].active = false;
        bullet[i].speed = 600.0f;
        bullet[i].radius = BULLET_RADIUS;
        bullet[i].damage = 100;
    }

    // Setup initial values for enemies
    Enemy enemy[5];
    for (int i = 0; i < 5; i++)
    {
        enemy[i].position.x = 100 + (i * 120);
        enemy[i].position.y = 50;
        enemy[i].direction;
        enemy[i].active = true;
        enemy[i].speed = 50.0f;
        enemy[i].health = 100;
        enemy[i].bounty = 10;
        enemy[i].rect = (Rectangle){ enemy[i].position.x, enemy[i].position.y, ENEMY_WIDTH, ENEMY_HEIGHT };
        enemy[i].rect.x = enemy[i].position.x;
        enemy[i].rect.y = enemy[i].position.y;
    }
    
    
    // Vector 2 position bounds
    Vector2 minBounds = { 0, 0 };
    Vector2 maxBounds = { screenWidth - PLAYER_WIDTH, screenHeight - PLAYER_HEIGHT };
    
    // Game Loop
    while (!WindowShouldClose()) {
        
        // Update Logic (Decision making based on State)
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
            if (IsMouseButtonPressed (MOUSE_LEFT_BUTTON)) 
            {
                for (int i = 0; i < MAX_BULLETS; i++) 
                {
                    if (!bullet[i].active) // Find bullet not active
                    { 
                        bullet[i].active = true;
                        bullet[i].position = playerCenter;
            
                        Vector2 target = GetMousePosition ();
                        Vector2 diff = Vector2Subtract (target, playerCenter);
                        bullet[i].direction = Vector2Normalize (diff);
            
                        break; // Stop in order to not shoot all 50 bullets
                    }
                }
            }

            // Update all bullets in the pool
            for (int i = 0; i < MAX_BULLETS; i++) 
            {
                // Only update the bullet if it is currently in flight
                if (bullet[i].active) 
                {
                    // Calculate new position based on direction and speed
                    bullet[i].position.x += bullet[i].direction.x * bullet[i].speed * GetFrameTime();
                    bullet[i].position.y += bullet[i].direction.y * bullet[i].speed * GetFrameTime();

                    // Check if the bullet has left the screen boundaries
                    // We include the radius to ensure it's completely out of sight before deactivating
                    if (bullet[i].position.x < -bullet[i].radius || 
                    bullet[i].position.x > screenWidth + bullet[i].radius ||
                    bullet[i].position.y < -bullet[i].radius || 
                    bullet[i].position.y > screenHeight + bullet[i].radius) 
                    {
                        // Set to false so this slot can be reused by a new shot
                        bullet[i].active = false;
                    }
                }
            }

            // Check collision: Bullets vs Enemies
            for (int i = 0; i < MAX_BULLETS; i++) 
            {
                if (bullet[i].active) 
                {
                    for (int j = 0; j < 5; j++) 
                    {
                        if (enemy[j].active) 
                        {
                            // Check if the bullet circle overlaps the enemy rectangle
                            if (CheckCollisionCircleRec (bullet[i].position, bullet[i].radius, enemy[j].rect)) 
                            {
                                // 1. Deactivate the bullet
                                bullet[i].active = false;
                    
                                // 2. Subtract damage from enemy health
                                enemy[j].health -= bullet[i].damage;
                    
                                // 3. Check if enemy is dead
                                if (enemy[j].health <= 0) 
                                {
                                    enemy[j].active = false;
                                    player.dollars += enemy[j].bounty; // Reward the player!
                                }
                    
                                break; // Exit the enemy loop since the bullet is gone
                            }
                        }
                    }
                }
            }
        
            // Player death
            if (player.health <= 0)
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
                currentState = STATE_MENU; // Back to MENU
                player.health = 100; // Revive player
                player.dollars = 0; // Reset player's dollars
                player.position = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }; // Reset player's spawn

                // Revive enemies and reset enemies' spawn
                for (int i = 0; i < 5; i++)
                {
                    enemy[i].health = 100;
                    enemy[i].active = true;
                    enemy[i].position.x = 100 + (i * 120);
                    enemy[i].position.y = 50;
                }
                
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
                float logoScale = 2.5f;
            
                ClearBackground (BEIGE); 
                DrawTextureEx (
                    logoTexture, 
                    (Vector2){ GetScreenWidth () / 2.0f - (logoTexture.width * logoScale) / 2.0f, GetScreenHeight () / 10.0f }, 
                    0.0f, 
                    logoScale, 
                    WHITE
                );

                const char *startText = "Press ENTER to start";
                int fontSize = 60;
                int textWidth = MeasureText (startText, fontSize);
                float alpha = (sinf (GetTime () * 2.0f) + 1.0f) / 2.0f;

                DrawText (startText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight () * 0.75f, fontSize, Fade (BLACK, alpha));
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

                // Draw the player
                DrawRectangleRec (player.rect, GREEN);

                // Draw player's HUD
                // Background bar
                DrawRectangleRec (playerHUD.backgroundBar, GRAY);

                // Health bar
                float healthPercent = (float)player.health / (float)player.maxHealth;
                playerHUD.healthBar.width = healthPercent * playerHUD.backgroundBar.width;

                Color healthColor = GREEN;
                if (healthPercent <= 0.5f && healthPercent > 0.3f) healthColor = YELLOW;
                else if (healthPercent <= 0.3f) healthColor = RED;
                DrawRectangleRec (playerHUD.healthBar, healthColor);
                
                // Dollars
                DrawText (
                    TextFormat ("$: %d", player.dollars), 
                    playerHUD.dollarsPosition.x, 
                    playerHUD.dollarsPosition.y, 
                    playerHUD.fontSize, 
                    playerHUD.moneyColor
                );

                // Loop through the bullet pool and draw each active bullet
                for (int i = 0; i < MAX_BULLETS; i++) 
                {
                    if (bullet[i].active) 
                    {
                        DrawCircleV (bullet[i].position, bullet[i].radius, BLACK);
                    }
                }

                // Draw the enemies
                for (int i = 0; i < 5; i++) 
                {
                    if (enemy[i].active) 
                    {
                        DrawRectangleRec (enemy[i].rect, RED);
                    }
                }
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