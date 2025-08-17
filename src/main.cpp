#include "raylib.h"
#include "Environment.h"
#include "StaticBody.h"

#include "Utils.h"

int main()
{
    
    const int SCREEN_WIDTH = 1920; 
    const int SCREEN_HEIGHT = 1080; 

    const std::string WIDOW_TITLE = "Testing Environment - " + GetCurrentTimeString();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WIDOW_TITLE.c_str());

    EnableCursor();

    SetTargetFPS(60);            

    Environment env;

    std::shared_ptr<StaticBody> staticBody = std::make_shared<StaticBody>(Rectangle{ 0, 0, 50, 50 });
    env.AddStaticObject(staticBody);

    while (!WindowShouldClose()) 
    {
        PollInputEvents();  // End of inputs for this frame

        BeginDrawing();

        ClearBackground(RAYWHITE);

        env.Draw();

        EndDrawing();
    }

    CloseWindow(); 
    

    return 0;
}