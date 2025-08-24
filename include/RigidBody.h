#pragma once

#include "raylib.h"

class RigidBody 
{
public:
    RigidBody() = default;

    RigidBody(const Rectangle& rect);

    RigidBody(const Rectangle& rect, const Color& color);

    virtual ~RigidBody() = default;

    virtual void Draw() const;

    Rectangle GetBounds() const;

protected:
    Rectangle bounds;

    Color color;
};