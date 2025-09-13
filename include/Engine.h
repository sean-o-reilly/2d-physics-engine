#pragma once

#include "raylib.h"
#include "Environment.h"

class Engine
{
public:
    void Run();

private:
    void Update(float& accumulator);

    Environment env;
    
    static const int screenWidth;
    static const int screenHeight;
    static const float fixedDeltaTime;
};