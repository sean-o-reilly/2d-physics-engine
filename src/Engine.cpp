#include "Engine.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <sstream>

const int Engine::screenWidth = 1920; 
const int Engine::screenHeight = 1080; 
const float Engine::fixedDeltaTime = 0.006f; // 6ms per frame

void Engine::Run()
{
    if (!SelectLoadedEnvironment(env))
    {
        return;
    }

    Environment initEnv(env); // Copy init state for resetting at runtime

    std::cout << "Enable benchmarking? (y/n): ";
    char benchmarkChoice;
    std::cin >> benchmarkChoice;

    bool benchmarkEnabled = false;
    if (benchmarkChoice == 'Y' || benchmarkChoice == 'y')
    {
        benchmarkEnabled = true;
    }

    std::ofstream benchmarkLog;
    std::string benchmarkFilePath;

    if (benchmarkEnabled)
    {
        std::filesystem::create_directories("logs");
        std::time_t now = std::time(nullptr);
        std::tm localTime;
        localtime_s(&localTime, &now);

        benchmarkFilePath = "logs/Benchmarks/Benchmark_" + GetCurrentTimeString() + ".txt";
        std::cout << benchmarkFilePath << std::endl;
        benchmarkLog.open(benchmarkFilePath, std::ios::app);
    }

    auto writeHourMinuteSecondToFile = [&benchmarkLog]() 
    {
        std::time_t now = std::time(nullptr);
        std::tm localTime;
        localtime_s(&localTime, &now);
        std::ostringstream oss;
        oss << "[" << GetCurrentTimeString() << "]";
        benchmarkLog << oss.str();
    };

    const std::string initTime = GetCurrentTimeString();
    const std::string windowTitle = ("2D Physics Engine - " + initTime);
    InitWindow(screenWidth, screenHeight, windowTitle.c_str());

    EnableCursor();

    SetTargetFPS(60);   

    float accumulator = 0.0f;

    unsigned int frameCount = 0;
    const unsigned int maxFrames = 1000; //only benchmark for maxFrames frames

    while (!WindowShouldClose()) 
    {
        if (IsKeyDown(KEY_R)) // Reset environment
        {
            env = initEnv;
        }

        if (benchmarkEnabled)
        {
            const std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            Update(accumulator);
            const std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<float, std::milli> duration = end - start;

            writeHourMinuteSecondToFile();
            benchmarkLog << " : " << duration.count() << " ms\n";

            ++frameCount;
            if (frameCount >= maxFrames) // Stop benchmarking after maxFrames
            {
                benchmarkEnabled = false;
                std::cout << "Benchmarking complete. Results saved to " << benchmarkFilePath << std::endl;
                benchmarkLog.close();
            }
        }
        else
        {
            Update(accumulator);
        }
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