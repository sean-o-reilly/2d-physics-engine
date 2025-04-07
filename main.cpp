#include "raylib.h"
#include "Entity.h"
#include "Utils.h"

#include <string>
#include <array>

/*TODO:
 *
 *  Condense main
 *  Move ALL debugging drawing utilities to the inside of entity, get them out of main
 *  *except for main debuggers, those can go in a different file
 *
 *  General enhance OOP in the codebase
 *   - Better data hiding
 *   - Setup polymorphism and abstracted base classes
 *   - Aim at making the draw call loop a bit cleaner and scalable
 *
 *  As of now the collision, friction, velocity and general environment is working
 *  fine, but I need to fix a small double collision off-by-1
 *  bug when on the corner of a platform. this is preventing circle from "rolling"
 *  off edges of platforms, but it can jump just fine
 *
 *  Setup better information/debugging HUD
 *
 */

//main debuggers
void showTime(int x, int y, int fontSize, Color color);
void showFPS(int x, int y, int fontSize, Color color);
void showDebug(int x, int y, int fontSize, Color color);

//should turn this into an abstracted base class with a variable shape
struct Obstacle {
    Rectangle rec;
    Color color;
};

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    const int screenWidth = 1920; //1280x720 for 720p
    const int screenHeight = 1200; //1920x1200 for fullscreen 16:10

    InitWindow(screenWidth, screenHeight, "Jumping Entity");
    EnableCursor();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    //--------------------------------------------------------------------------------------
    // Creating objects


    Entity circle(50, BLUE);
    Rectangle platform = Rectangle{screenWidth * .2, screenHeight / 1.5, screenWidth * .45, 50};
    Rectangle floor = Rectangle{0, screenHeight - 50, screenWidth, 50};
    Rectangle wall = Rectangle{1920 - 450, 750, 100, 400};
    Rectangle wall2 = Rectangle{platform.x - 100, platform.y - 400, 100, 400};

    //make obstacles struct with Rectangle and color pair
    // ^^

    // Rectangle obstacles[10];
    const int objCount = 4;
    std::array<Rectangle, objCount> obstacles;

    obstacles[0] = platform;
    obstacles[1] = floor;
    obstacles[2] = wall;
    obstacles[3] = wall2;

    bool showDebugMenu = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        //user control + object interaction
        circle.Update(obstacles.data(), objCount);

        //toggle debuging menu
        if (GetKeyPressed() == 'P') showDebugMenu = !showDebugMenu;

        PollInputEvents();  //end of inputs for this frame

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        if (IsCursorOnScreen()) {HideCursor();} //get rid of cursor

        ClearBackground(RAYWHITE);

        if (showDebugMenu) {
            showDebug(10, 10, 30, BLACK);
            //put debug entity function here
        }

        DrawRectangleRec(obstacles[0], BLACK);
        DrawRectangleRec(obstacles[1], GRAY);
        DrawRectangleRec(obstacles[2], GREEN);
        DrawRectangleRec(obstacles[3], RED);

        circle.Draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}



//main debuggeres
void showTime(int x, int y, int fontSize, Color color) {
    const char* timeStr = std::to_string(GetTime()).c_str();


    DrawText("Time: ", x, y, fontSize, color);
    DrawText(timeStr, x+ (fontSize * 3), y, fontSize, color);
}

void showFPS(int x, int y, int fontSize, Color color) {
    int fps = GetFPS();
    const char* fpsStr = std::to_string(fps).c_str();
    Color fpsColor;
    if (fps >= 60) fpsColor = GREEN;
    else if (fps >= 35) fpsColor = YELLOW;
    else fpsColor = RED;

    DrawText("FPS: ", x, y, fontSize, color);
    DrawText(fpsStr, x + (3 * fontSize), y, fontSize, fpsColor);
}

void showDebug(int x, int y, int fontSize, Color color) {
    int lines = 2;

    showTime(x, y, fontSize, color);
    showFPS(x, y + fontSize, fontSize, color);

    Rectangle border = Rectangle{x - (fontSize / 4), y - (fontSize / 4),
        fontSize * 10, (lines * fontSize) + (fontSize / 2)};

    DrawRectangleLinesEx(border, 3, color);
}

