#include "raylib.h"
#include "Entity.h"
#include "Utils.h"

#include <string>
#include <array>

struct Obstacle {
    Rectangle rec;
    Color color;
};

Texture2D background;

int main()
{
    const int screenWidth = 1920; 
    const int screenHeight = 1200; 

    InitWindow(screenWidth, screenHeight, "Testing Environment");
    EnableCursor();

    SetTargetFPS(60);            

    Entity user(50, BLUE);

    const int objCount = 4;
    std::array<Rectangle, objCount> obstacles;

    Rectangle platform = Rectangle{(float)(screenWidth * .2), (float)(screenHeight / 1.5), (float)(screenWidth * .45), 50.0f};
    Rectangle floor = Rectangle{0.0f, (float)(screenHeight - 50), (float)screenWidth, 50.0f};
    Rectangle wall = Rectangle{(float)(1920 - 450), 750.0f, 100.0f, 400.0f};
    Rectangle wall2 = Rectangle{platform.x - 100.0f, platform.y - 400.0f, 100.0f, 400.0f};

    obstacles[0] = platform;
    obstacles[1] = floor;
    obstacles[2] = wall;
    obstacles[3] = wall2;

    bool showDebugMenu = false;

    while (!WindowShouldClose()) 
    {
        user.Update(obstacles.data(), objCount);

        if (GetKeyPressed() == 'P') showDebugMenu = !showDebugMenu;

        PollInputEvents();  // End of inputs for this frame

        BeginDrawing();

            if (IsCursorOnScreen()) {HideCursor();}

            ClearBackground(RAYWHITE);
            DrawTextureV(background, Vector2{0.0f,0.0f},WHITE);

            DrawRectangleRec(obstacles[0], BLACK);
            DrawRectangleRec(obstacles[1], GRAY);
            DrawRectangleRec(obstacles[2], GREEN);
            DrawRectangleRec(obstacles[3], RED);

            user.Draw();

            if (showDebugMenu) {
                showDebugMain(10, 10, 30, GREEN);
                for (Rectangle rec : obstacles) {
                    DrawRectangleLinesEx(rec, 5, RED);
                }
                showDebugEntity(user, Vector2{(float)(user.getXPos() - 100), (float)(user.getYPos() - 200)}, 20, BLUE);
            }
        EndDrawing();
    }

    CloseWindow(); 

    return 0;
}