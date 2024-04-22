#include "raylib.h"
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define SNAKE_SIZE 20
#define FOOD_SIZE 20
#define SNAKE_INITIAL_LENGTH 1
#define MOVE_SPEED 20
#define SNAKE_MAX_LENGTH ((SCREEN_WIDTH * SCREEN_HEIGHT) / (SNAKE_SIZE * SNAKE_SIZE))

typedef enum GameScreen {
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
} GameScreen;

typedef struct Snake {
    Vector2* body;
    int bodySize;
    int maxLength;
    Vector2 direction;
    int score;
} Snake;

typedef struct Food {
    Vector2 position;
} Food;

static Texture2D texLogo;

static Font font;

static Snake snake;
static Food food;

static GameScreen screen;
static int framesCounter;

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void DeInitGame(void);

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SNAKE");
    SetTargetFPS(60);

    InitAudioDevice();
    InitGame();

    while (!WindowShouldClose())
    {
        UpdateGame();
        BeginDrawing();
            DrawGame();
        EndDrawing();
    }

    DeInitGame();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

static void InitGame(void) {
    #ifndef RESOURCES
    #define RESOURCES
        texLogo = LoadTexture("resources/raylib_logo.png");
        font = LoadFont("resources/setback.png");
    #endif

    screen = LOGO;
    framesCounter = 0;

    snake.bodySize = SNAKE_INITIAL_LENGTH;
    snake.maxLength = SNAKE_MAX_LENGTH;
    snake.body = (Vector2*) malloc(snake.maxLength * sizeof(Vector2));
    snake.body[0] = (Vector2){
        SNAKE_SIZE * (SCREEN_WIDTH / 2 / SNAKE_SIZE), 
        SNAKE_SIZE * (SCREEN_HEIGHT / 2 / SNAKE_SIZE)
    };
    snake.direction = (Vector2){0, 0};
    snake.score = 0;

    food.position = (Vector2){
        SNAKE_SIZE * (GetRandomValue(0, (SCREEN_WIDTH - FOOD_SIZE) / SNAKE_SIZE)), 
        SNAKE_SIZE * (GetRandomValue(0, (SCREEN_HEIGHT - FOOD_SIZE) / SNAKE_SIZE))
    };
}

static void UpdateGame(void) {
    switch(screen) {
        case LOGO: {
            framesCounter++;

            if (framesCounter > 180) {
                screen = TITLE;
                framesCounter = 0;
            }
        } break;
        case TITLE: {
            framesCounter++;

            if (IsKeyPressed(KEY_ENTER)){
                screen = GAMEPLAY;
            }
        } break;
        case GAMEPLAY: {
            if(IsKeyPressed(KEY_ENTER)) {
                screen = ENDING;
            }
        } break;
        case ENDING: {
            framesCounter++;

            if(IsKeyPressed(KEY_ENTER)) {
                InitGame();
                screen = TITLE;
            }
        } break;
        default: break;
    }
}

static void DrawGame(void) {
    ClearBackground(RAYWHITE);

    switch(screen) {
        case LOGO: {
            DrawTexture(texLogo, SCREEN_WIDTH / 2 - texLogo.width / 2, SCREEN_HEIGHT / 2 - texLogo.height / 2, WHITE);      
        } break;
        case TITLE: {
            DrawTextEx(font, "SNAKE", (Vector2){SCREEN_WIDTH / 2 - MeasureText("SNAKE", 80) / 2, 80}, 80, 10, MAROON);
            
            if((framesCounter / 30) % 2 == 0) {
                DrawText("PRESS [ENTER] TO START", 
                    SCREEN_WIDTH / 2 - MeasureText("PRESS [ENTER] TO START", 20) / 2,
                    SCREEN_HEIGHT / 2 + 60,
                    20,
                    DARKGRAY);
            }
        } break;
        case GAMEPLAY: {
            ClearBackground(BLACK);

            DrawRectangleV(snake.body[0], (Vector2){SNAKE_SIZE, SNAKE_SIZE}, DARKGREEN);
            for (int i = 1; i < snake.bodySize; i++) {
                DrawRectangleV(snake.body[i], (Vector2){SNAKE_SIZE, SNAKE_SIZE}, GREEN);
            }

            DrawRectangleV(food.position, (Vector2){FOOD_SIZE, FOOD_SIZE}, RED);
        } break;
        case ENDING: {
            DrawTextEx(font, TextFormat("SCORE: %d", snake.score), (Vector2){SCREEN_WIDTH / 2 - MeasureText("END", 80) / 2, 100}, 80, 6, MAROON);
                        
            if ((framesCounter) % 2 == 0) {
                DrawText("PRESS [ENTER] TO PLAY AGAIN", 
                    GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
                    GetScreenHeight() / 2 + 80,
                    20,
                    GRAY);
            }
        } break;
        default: break;
    }
}

static void DeInitGame(void) {
    UnloadTexture(texLogo);
    UnloadFont(font);
}
