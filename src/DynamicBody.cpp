#include "DynamicBody.h"
#include "raymath.h"

#include <algorithm>

const Color DynamicBody::defaultColor = GREEN;
const float DynamicBody::defaultRestitution = 0.1f;
const float DynamicBody::sleepThreshold = 50.0f;

void DynamicBody::Update(const float deltaTime)
{
    bounds.x += velocity.x * deltaTime;
    bounds.y += velocity.y * deltaTime;
}

void DynamicBody::ApplyAcceleration(const Vector2 vel)
{
    velocity = Vector2Add(velocity, vel);
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

DynamicBody::DynamicBody(const Rectangle& rect)
    : RigidBody{rect}, velocity({0.0f, 0.0f}),
    restitution{defaultRestitution}
    {}

DynamicBody::DynamicBody(const Rectangle& rect, const Color& color, const float friction, const float restitution)
    : RigidBody(rect, color, friction), velocity({0.0f, 0.0f}),
    restitution{restitution}
    {}