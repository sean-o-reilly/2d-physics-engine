#pragma once

#include "RigidBody.h"
#include "raylib.h"

class DynamicBody : public RigidBody
{
public:
    void Update(const float deltaTime);

    void ApplyAcceleration(const Vector2 vel);

    Rectangle GetBounds() const;

    void SetPositionY(float y);
    void SetPositionX(float x);
    
    Vector2 velocity;

    const float restitution;

private:
    static const float sleepThreshold;

public:
    DynamicBody(const Rectangle& rect);
    
    DynamicBody(const Rectangle& rect, const Color& color, const float friction, const float restitution);

    static const Color defaultColor;
    static const float defaultRestitution;
    
private:
    DynamicBody() = delete;
};