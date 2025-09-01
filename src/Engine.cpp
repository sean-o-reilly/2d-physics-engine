#include "Engine.h"

const int Engine::screenWidth = 1920; 
const int Engine::screenHeight = 1080; 
const float Engine::fixedDeltaTime = 0.006f; // 6ms per frame

void Engine::Run()
{
    if (!SelectLoadedEnvironment(env))
    {
        return;
    }

    Environment initEnv(env); // Copy for resetting at runtime

    const std::string windowTitle = ("2D Physics Engine - " + GetCurrentTimeString());
    InitWindow(screenWidth, screenHeight, windowTitle.c_str());

    EnableCursor();

    SetTargetFPS(60);   

    float accumulator = 0.0f;

    while (!WindowShouldClose()) 
    {
        if (IsKeyDown(KEY_R)) // Reset environment
        {
            env = initEnv;
        }

        Update(accumulator);
    }

    CloseWindow(); 
}

void Engine::Update(float& accumulator)
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