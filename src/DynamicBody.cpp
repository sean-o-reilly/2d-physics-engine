#include "DynamicBody.h"
#include "raymath.h"

const Color DynamicBody::defaultColor = GREEN;

void DynamicBody::Update(const float deltaTime)
{
    bounds.x += velocity.x * deltaTime;
    bounds.y += velocity.y * deltaTime;
}

void DynamicBody::ApplyAcceleration(const Vector2& vel)
{
    velocity = Vector2Add(velocity, vel);
}

Vector2 DynamicBody::GetVelocity() const
{
    return velocity;
}

Rectangle DynamicBody::GetBounds() const
{
    return bounds;
}

void DynamicBody::SetPositionY(float y)
{
    bounds.y = y;
}

void DynamicBody::SetPositionX(float x)
{
    bounds.x = x;
}

void DynamicBody::SetVelocityY(float y)
{
    velocity.y = y;
}

void DynamicBody::SetVelocityX(float x)
{
    velocity.x = x;
}

DynamicBody::DynamicBody(const Rectangle& rect)
    : RigidBody(rect), velocity({0.0f, 0.0f}) {}

DynamicBody::DynamicBody(const Rectangle& rect, const Color& color)
    : RigidBody(rect, color), velocity({0.0f, 0.0f}) {}