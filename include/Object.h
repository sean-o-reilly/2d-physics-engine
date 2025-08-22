#pragma once
#include "raylib.h"

class Object 
{

public:
    virtual void Draw() const = 0;

    Object() = default;

    Object(const Rectangle& rect);

    Object(const Rectangle& rect, const Color& color);

    virtual ~Object() = default;
protected:
    Rectangle bounds;

    Color color;
};