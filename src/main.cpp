#include "raylib.h"
#include "Environment.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
    Environment loadedEnv;
    SelectLoadedEnvironment(loadedEnv);

    const int screenWidth = 1920; 
    const int screenHeight = 1080; 

    const std::string windowTitle("Testing Environment - " + GetCurrentTimeString());
    InitWindow(screenWidth, screenHeight, windowTitle.c_str());

    EnableCursor();

    SetTargetFPS(60);   

    while (!WindowShouldClose()) 
    {
        loadedEnv.Update();

        PollInputEvents();  // End of inputs for this frame
        
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        loadedEnv.Draw();

        EndDrawing();
    }

    CloseWindow(); 
    
    return 0;
}