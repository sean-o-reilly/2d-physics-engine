#pragma once

#include "RigidBody.h"
#include "raylib.h"

class DynamicBody : public RigidBody
{
public:
    void Update(const float deltaTime);

    void ApplyAcceleration(const Vector2& vel);

    Vector2 GetVelocity() const;

    Rectangle GetBounds() const;

    void SetPositionY(float y);
    void SetPositionX(float x);
    
    void SetVelocityY(float y);
    void SetVelocityX(float x);
    
    DynamicBody(const Rectangle& rect);
    
    DynamicBody(const Rectangle& rect, const Color& color);
    
    static const Color defaultColor;

private:
    DynamicBody() = delete;

    Vector2 velocity;
};