#include "DynamicBody.h"
#include "raymath.h"

#include <algorithm>

const Color DynamicBody::defaultColor = GREEN;
const float DynamicBody::defaultRestitution = 0.1f;
const float DynamicBody::sleepThreshold = 0.5f;

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

void DynamicBody::TryToSleep()
{
    if (std::abs(velocity.x) < sleepThreshold && std::abs(velocity.y) < sleepThreshold)
    {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        isAsleep = true;
    }
    else
    {
        isAsleep = false;
    }
}

DynamicBody::DynamicBody(const Rectangle& rect)
    : RigidBody{rect}, velocity({0.0f, 0.0f}),
    restitution{defaultRestitution}, isAsleep{false}
    {}

DynamicBody::DynamicBody(const Rectangle& rect, const Color& color, const float restitution)
    : RigidBody(rect, color), velocity({0.0f, 0.0f}),
    restitution{restitution}, isAsleep{false}
    {}