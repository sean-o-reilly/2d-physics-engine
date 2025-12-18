#include "Engine.h"
#include "Utils.h"
#include "Serializer.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <string>

const int Engine::screenWidth = 1920; 
const int Engine::screenHeight = 1080; 
constexpr float Engine::fixedDeltaTime = 0.006f;

void Engine::Run()
{
    while (Utils::SelectLoadedEnvironment(env))
    {
        Environment initEnv(env); // Copy init state for resetting at runtime
    
        std::cout << "Enable benchmarking? (y): ";
        std::string benchmarkChoice;
        std::getline(std::cin, benchmarkChoice);
    
        bool benchmarkEnabled = benchmarkChoice == "y";
    
        std::ofstream benchmarkLog;
        std::string benchmarkFilePath;
    
        if (benchmarkEnabled)
        {
            std::filesystem::create_directories("logs/Benchmarks");
            std::time_t now = std::time(nullptr);
            std::tm localTime;
            localtime_s(&localTime, &now);

            benchmarkFilePath = "logs/Benchmarks/Benchmark_" + Utils::GetCurrentTimeString() + ".txt";
            std::cout << benchmarkFilePath << std::endl;
            benchmarkLog.open(benchmarkFilePath, std::ios::app);
        }
            
        InitWindow(screenWidth, screenHeight, "Physics Engine");
        EnableCursor();
        SetTargetFPS(60);   
        
        float accumulator = 0.0f;
        
        unsigned int frameCount = 0;
        constexpr unsigned int maxFrames = 500; //only benchmark for maxFrames frames
        constexpr float timeoutMs = 150.0f;

        while (!WindowShouldClose()) 
        {
            if (IsKeyDown(KEY_R)) // Reset environment
            {
                env = initEnv;
            }
    
            if (benchmarkEnabled)
            {
                UpdateTimed(accumulator, benchmarkLog, timeoutMs);
                ++frameCount;
                if (frameCount >= maxFrames) // Stop benchmarking after maxFrames
                {
                    std::cout << "Benchmarking complete. Results saved to " << benchmarkFilePath << std::endl;
                    benchmarkLog.close();
                    break;
                }
            }
            else
            {
                Update(accumulator);
            }
        }
        std::cout << "Exiting environment" << std::endl;
        CloseWindow(); 
    }
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

void Engine::UpdateTimed(float& accumulator, std::ofstream& benchmarkLog, float timeoutThreshold)
{
    const std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    Update(accumulator);
    const std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float, std::milli> duration = end - start;

    if (duration.count() > timeoutThreshold)
    {
        std::string errMsg = "Frametime exceeded timeout threshold of " +
            std::to_string((int)timeoutThreshold) + "ms. Engine runtime killed.";
        
        std::cerr << errMsg << std::endl;
        throw std::runtime_error(errMsg);
    }

    Utils::WriteTimeToFile(benchmarkLog);
    benchmarkLog << " : " << duration.count() << " ms\n";
}