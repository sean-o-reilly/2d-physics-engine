#include "RigidBody.h"

RigidBody::RigidBody(const Rectangle& rect) 
: bounds(rect), friction(defaultFriction) {}

RigidBody::RigidBody(const Rectangle& rect, const Color& color, const float friction)
    : bounds(rect), color(color), friction(friction) {}

void RigidBody::Draw() const
{
    DrawRectangleRec(bounds, color);
}

Rectangle RigidBody::GetBounds() const
{
    return bounds;
}

float RigidBody::GetFriction() const
{
    return friction;
}