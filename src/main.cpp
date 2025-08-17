#include "raylib.h"
#include "Environment.h"
#include "StaticBody.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
    Environment loadedEnv = SelectLoadedEnvironment();

    const int SCREEN_WIDTH = 1920; 
    const int SCREEN_HEIGHT = 1080; 

    const std::string WIDOW_TITLE = "Testing Environment - " + GetCurrentTimeString();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WIDOW_TITLE.c_str());

    EnableCursor();

    SetTargetFPS(60);   

    while (!WindowShouldClose()) 
    {
        PollInputEvents();  // End of inputs for this frame

        BeginDrawing();

        ClearBackground(RAYWHITE);

        loadedEnv.Draw();

        EndDrawing();
    }

    CloseWindow(); 
    
    return 0;
}