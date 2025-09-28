#pragma once

#include "raylib.h"

class RigidBody 
{
public:

    RigidBody(const Rectangle& rect);

    RigidBody(const Rectangle& rect, const Color& color);

    void Draw() const;

    Rectangle GetBounds() const;

protected:
    Rectangle bounds;

    Color color;

private:
    RigidBody() = delete;

};