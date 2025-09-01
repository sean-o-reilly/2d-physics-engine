#pragma once

#include "raylib.h"
#include "Environment.h"
#include "Utils.h"

class Engine
{
public:
    void Run();

private:
    void Update(const float fixedDeltaTime, float& accumulator);

    Environment env;
    
    static const int screenWidth;
    static const int screenHeight;
};