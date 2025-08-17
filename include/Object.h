#pragma once
#include "raylib.h"

class Object 
{

public:
    virtual void Draw() const = 0;

    Object() = default;

    Object(const Rectangle& rect);

    virtual ~Object() = default;
protected:
    Rectangle bounds;
};