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
    while (SelectLoadedEnvironment(env))
    {
        Environment initEnv(env); // Copy init state for resetting at runtime
    
        std::cout << "Enable benchmarking? (y/n): ";
        std::string benchmarkChoice;
        std::getline(std::cin, benchmarkChoice);
    
        bool benchmarkEnabled = false;
        if (benchmarkChoice == "y")
        {
            benchmarkEnabled = true;
        }
    
        std::string engineStartTime = GetCurrentTimeString();
    
        std::ofstream benchmarkLog;
        std::string benchmarkFilePath;
    
        if (benchmarkEnabled)
        {
            std::filesystem::create_directories("logs");
            std::time_t now = std::time(nullptr);
            std::tm localTime;
            localtime_s(&localTime, &now);
    
            benchmarkFilePath = "logs/Benchmarks/Benchmark_" + engineStartTime + ".txt";
            std::cout << benchmarkFilePath << std::endl;
            benchmarkLog.open(benchmarkFilePath, std::ios::app);
        }
            
        InitWindow(screenWidth, screenHeight, "Physics Engine");
        
        EnableCursor();
        
        SetTargetFPS(60);   
        
        float accumulator = 0.0f;
        
        unsigned int frameCount = 0;
        const unsigned int maxFrames = 1000; //only benchmark for maxFrames frames
    
        auto writeHourMinuteSecondToFile = [&benchmarkLog]()
        {
            std::time_t now = std::time(nullptr);
            std::tm localTime;
            localtime_s(&localTime, &now);
            std::ostringstream oss;
            oss << "[" << GetCurrentTimeString() << "]";
            benchmarkLog << oss.str();
        };
    
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
    
                if (duration.count() > 150)
                {
                    const char* errMsg = "Frametime exceeded 150ms. Engine runtime killed.";
                    std::cerr << errMsg << std::endl;
                    throw std::runtime_error(errMsg);
                }
    
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