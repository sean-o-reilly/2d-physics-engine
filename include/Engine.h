#pragma once

#include "raylib.h"
#include "Environment.h"

#include <fstream>

class Engine
{
public:
    void Run();

private:
    void Update(float& accumulator);

    void UpdateTimed(float& accumulator, std::ofstream& benchmarkLog, float timeoutThreshold);

    Environment env;
    
    static const int screenWidth;
    static const int screenHeight;
    static const float fixedDeltaTime;
};