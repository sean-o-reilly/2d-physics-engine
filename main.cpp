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
Texture2D scorpion;

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    const int screenWidth = 1200; //1280x720 for 720p
    const int screenHeight = 675; //1920x1200 for fullscreen 16:10



    InitWindow(screenWidth, screenHeight, "Pit 1");
    EnableCursor();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    //--------------------------------------------------------------------------------------


    //Creating environment for pit 1
    background = LoadTexture("C:/Users/oreil/CLionProjects/2d-physics-engine/sprites/pit1.jpg"); //must be loaded after window init
    assert(IsTextureValid(background));

    scorpion = LoadTexture("C:/Users/oreil/CLionProjects/2d-physics-engine/sprites/scorpion_idle.png");
    Entity user(50, 296, 168, BLUE, scorpion);

    const int objCount = 1;
    std::array<Rectangle, objCount> obstacles;
    Rectangle platform = Rectangle{0, 575, screenWidth, 25};
    obstacles[0] = platform;

    bool showDebugMenu = false;
    //

    // Main rendering loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //---------------------------------------------------------------------------------

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

            if (showDebugMenu) {
                showDebugMain(10, 10, 30, GREEN);
                showDebugEntity(user, Vector2{user.getXPos() - user.getHitbox().width, user.getYPos() - user.getHitbox().height}, 20, BLUE);

                for (Rectangle rec : obstacles) {
                    DrawRectangleLinesEx(rec, 5, RED);
                }
            }

            user.Draw();

            if (GetTime() < 5.0f) {
                DrawText("FIGHT!", screenWidth/2 - 150, screenHeight/2 - 50, 130, RED);
            }
            else {
                DrawText(std::to_string(90 + 5 - (int)GetTime()).c_str(), screenWidth/2, 20, 65, RED);
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

