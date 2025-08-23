#pragma once

#include "raylib.h"

class RigidBody 
{
public:
    virtual void Draw() const = 0;

    RigidBody() = default;
    RigidBody(const Rectangle& rect);
    RigidBody(const Rectangle& rect, const Color& color);
    virtual ~RigidBody() = default;
protected:
    Rectangle bounds;
    Color color;
};
