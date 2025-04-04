#include "raylib.h"
#include <string>
#include "Entity.h"

Rectangle obstacle;

/*TODO:
 *
 *  Condense main
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

//have to get this sstuff out of main into a debugging utils file or class
void showPosition(Entity circle);
void showVelocity(Entity circle);
void showCollision(Entity circle);
void showEnvFloor(Entity circle);
void showEnvCeiling(Entity circle);
void showEnvLeft(Entity circle);
void showEnvRight(Entity circle);

void showTime(int x, int y, int fontSize, Color color);
void showFPS(int x, int y, int fontSize, Color color);

void showDebug(int x, int y, int fontSize, Color color);

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

    Rectangle obstacles[10];
    obstacles[0] = platform;
    obstacles[1] = floor;
    obstacles[2] = wall;
    obstacles[3] = wall2;

    int objCount = 4;

    bool showDebugMenu = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        //user control + object interaction
        circle.Update(obstacles, objCount);

        //toggle debuging menu
        if (GetKeyPressed() == 'P') showDebugMenu = !showDebugMenu;

        PollInputEvents();  //end of inputs for this frame

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        if (IsCursorOnScreen()) {HideCursor();} //get rid of cursor

        ClearBackground(RAYWHITE);

        //utilities
        // showPosition(circle);
        // showVelocity(circle);
        // showCollision(circle);
        // showEnvFloor(circle);
        // showEnvCeiling(circle);
        // showEnvLeft(circle);
        // showEnvRight(circle);

        if (showDebugMenu) showDebug(10, 10, 30, BLACK);

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

void showPosition(Entity circle) {
    const char* xStr = std::to_string(circle.getXPos()).c_str();
    const char* yStr = std::to_string(circle.getYPos()).c_str();
    DrawText("xPos: ", 0, 0, 20, BLACK);
    DrawText("yPos: ", 0, 20, 20, BLACK);
    DrawText(xStr, 60, 0, 20, BLACK);
    DrawText(yStr, 60, 20, 20, BLACK);
};
void showVelocity(Entity circle) {
    const char* xVelocityStr = std::to_string(circle.getXVelocity()).c_str();
    const char* yVelocityStr = std::to_string(circle.getYVelocity()).c_str();
    DrawText("xVelocity: ", 0, 60, 20, BLACK);
    DrawText("yVelocity: ", 0, 80, 20, BLACK);
    DrawText(xVelocityStr, 110, 60, 20, BLACK);
    DrawText(yVelocityStr, 110, 80, 20, BLACK);
};
void showCollision(Entity circle) {
    const char* xCollisionStr = std::to_string(circle.checkCollisionX()).c_str();
    const char* floorCollisionStr = circle.checkCollisionFloor() ? "true" : "false";
    const char* ceilingCollisionStr = circle.checkCollisionCeiling() ? "true" : "false";

    DrawText("X Collision: ", 0, 120, 20, BLACK);
    DrawText("Floor Collision: ", 0, 140, 20, BLACK);
    DrawText("Ceiling Collision: ", 0, 160, 20, BLACK);

    DrawText(xCollisionStr, 115, 120, 20, BLACK);
    DrawText(floorCollisionStr, 165, 140, 20, BLACK);
    DrawText(ceilingCollisionStr, 165, 160, 20, BLACK);
};


void showEnvFloor (Entity circle) {
    const char* envFlStr = std::to_string(circle.getEnvFloor()).c_str();


    DrawText("Entity Env Floor: ", 0, 240, 20, BLACK);
    DrawText(envFlStr, 190, 240, 20, BLACK);
    if (circle.getEnvFloor() == GetScreenHeight()) {
        const char* envIsScreenHeight = "(Screen Height)";
        DrawText(envIsScreenHeight, 310, 240, 20, BLACK);
    }
}

void showEnvCeiling (Entity circle) {
    const char* envClStr = std::to_string(circle.getEnvCeiling()).c_str();


    DrawText("Entity Env Ceiling: ", 0, 260, 20, BLACK);
    DrawText(envClStr, 190, 260, 20, BLACK);
    if (circle.getEnvFloor() == 0) {
        const char* envIsScreenHeight = "(Screen Height)";
        DrawText(envIsScreenHeight, 310, 260, 20, BLACK);
    }
}

void showEnvLeft (Entity circle) {
    const char* envClStr = std::to_string(circle.getEnvLeft()).c_str();


    DrawText("Entity Env Left: ", 0, 280, 20, BLACK);
    DrawText(envClStr, 190, 280, 20, BLACK);
}

void showEnvRight (Entity circle) {
    const char* envClStr = std::to_string(circle.getEnvRight()).c_str();


    DrawText("Entity Env Right: ", 0, 300, 20, BLACK);
    DrawText(envClStr, 190, 300, 20, BLACK);
}

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

