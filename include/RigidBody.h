#pragma once

#include "raylib.h"

class RigidBody 
{
public:

    RigidBody(const Rectangle& rect);

    RigidBody(const Rectangle& rect, const Color& color, const float friction);

    void Draw() const;

    Rectangle GetBounds() const;

    float GetFriction() const;
    
    static inline constexpr float defaultFriction = 0.25f;
protected:
    Rectangle bounds;
    
    Color color;
    
    const float friction;
    
private:
    RigidBody() = delete;
    
};