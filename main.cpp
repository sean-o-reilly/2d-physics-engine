#include "raylib.h"
#include "Entity.h"
#include "Utils.h"
#include <cassert>


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

//should turn this into an abstracted base class with a variable shape
struct Obstacle {
    Rectangle rec;
    Color color;
};

Texture2D background;

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    const int screenWidth = 1920; //1280x720 for 720p
    const int screenHeight = 1200; //1920x1200 for fullscreen 16:10


    InitWindow(screenWidth, screenHeight, "Testing Environment");
    EnableCursor();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    //--------------------------------------------------------------------------------------
    // Creating objects


    Entity user(50, BLUE);

    //Creating environment/scene for testing area

    const int objCount = 4;
    std::array<Rectangle, objCount> obstacles;

    Rectangle platform = Rectangle{screenWidth * .2, screenHeight / 1.5, screenWidth * .45, 50};
    Rectangle floor = Rectangle{0, screenHeight - 50, screenWidth, 50};
    Rectangle wall = Rectangle{1920 - 450, 750, 100, 400};
    Rectangle wall2 = Rectangle{platform.x - 100, platform.y - 400, 100, 400};

    obstacles[0] = platform;
    obstacles[1] = floor;
    obstacles[2] = wall;
    obstacles[3] = wall2;

    bool showDebugMenu = false;

    // Main rendering loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        //user control + object interaction
        user.Update(obstacles.data(), objCount);

        //toggle debuging menu
        if (GetKeyPressed() == 'P') showDebugMenu = !showDebugMenu;

        PollInputEvents();  //end of inputs for this frame

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            if (IsCursorOnScreen()) {HideCursor();} //get rid of cursor

            ClearBackground(RAYWHITE);
            DrawTextureV(background, Vector2{0,0},WHITE);

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
                showDebugEntity(user, Vector2{user.getXPos() - 100, user.getYPos() - 200}, 20, BLUE);


            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

