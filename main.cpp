#include "raylib.h"
#include <string>
#include "Circle.h"

Rectangle obstacle;

/*TODO:
 *
 *
 *  Add objects, then fix up circle to check collision not for ends of screen but for objects
 *  Out of bounds stuff?
 *  An array of objects that we can check for each time collision is called? or is that
 *  really suboptimal
 *
 *  FIX FREAKING COLLISION
 *
 *
 */

/*FIXED:
 *
 * Wall running bugs
 * floor collision glitching
 *
 */

void showPosition(Circle circle);
void showVelocity(Circle circle);
void showCollision(Circle circle);
void showTime();


int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    const int screenWidth = 1280; //1280x720 for 720p
    const int screenHeight = 720; //1920x1200 for fullscreen 16:10

    InitWindow(screenWidth, screenHeight, "Jumping Circle");
    EnableCursor();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Creating objects


    Circle circle(50, BLUE);
    Rectangle platform = Rectangle{screenWidth * .2, screenHeight / 2, screenWidth * .6, 50};



    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------



        if (CheckCollisionRecs(platform, circle.getHitbox())) {
            // circle.setColor(PURPLE);

        }
        else {
            circle.setColor(BLUE);
        }

        circle.Update();


        PollInputEvents();  //end of inputs for this frame

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        if (IsCursorOnScreen()) {HideCursor();} //get rid of cursor

        ClearBackground(RAYWHITE);

        //showPosition(circle);
        //showVelocity(circle);
        //showCollision(circle);
        //showTime();

        // circle.DrawHitbox();

        DrawRectangleRec(platform, BLACK);

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

void showPosition(Circle circle) {
    const char* xStr = std::to_string(circle.getXPos()).c_str();
    const char* yStr = std::to_string(circle.getYPos()).c_str();
    DrawText("xPos: ", 0, 0, 20, BLACK);
    DrawText("yPos: ", 0, 20, 20, BLACK);
    DrawText(xStr, 60, 0, 20, BLACK);
    DrawText(yStr, 60, 20, 20, BLACK);
};
void showVelocity(Circle circle) {
    const char* xVelocityStr = std::to_string(circle.getXVelocity()).c_str();
    const char* yVelocityStr = std::to_string(circle.getYVelocity()).c_str();
    DrawText("xVelocity: ", 0, 60, 20, BLACK);
    DrawText("yVelocity: ", 0, 80, 20, BLACK);
    DrawText(xVelocityStr, 110, 60, 20, BLACK);
    DrawText(yVelocityStr, 110, 80, 20, BLACK);
};
void showCollision(Circle circle) {
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
void showTime() {
    const char* timeStr = std::to_string(GetTime()).c_str();


    DrawText("Time: ", 0, 200, 20, BLACK);
    DrawText(timeStr, 65, 200, 20, BLACK);
}
