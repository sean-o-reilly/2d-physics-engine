#include "Engine.h"

const int Engine::screenWidth = 1920; 
const int Engine::screenHeight = 1080; 

void Engine::Run()
{
    if (!SelectLoadedEnvironment(env))
    {
        return;
    }

    const std::string windowTitle = ("2D Physics Engine - " + GetCurrentTimeString());
    InitWindow(screenWidth, screenHeight, windowTitle.c_str());

    EnableCursor();

    SetTargetFPS(60);   

    const float fixedDeltaTime = 0.006f; // 6ms per frame
    float accumulator = 0.0f;

    while (!WindowShouldClose()) 
    {
        Update(fixedDeltaTime, accumulator);
    }

    CloseWindow(); 
}

void Engine::Update(const float fixedDeltaTime, float& accumulator)
{
    float frameTime = GetFrameTime();
    accumulator += frameTime;

    while (accumulator >= fixedDeltaTime) // Fixed timestep
    {
        env.Update(fixedDeltaTime);
        accumulator -= fixedDeltaTime;
    }

    PollInputEvents();  // End of inputs for this frame
    
    BeginDrawing();
    ClearBackground(BLACK);
    env.Draw();
    EndDrawing();
}