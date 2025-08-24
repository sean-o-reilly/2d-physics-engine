#include "RigidBody.h"

RigidBody::RigidBody(const Rectangle& rect) 
: bounds(rect) {}

RigidBody::RigidBody(const Rectangle& rect, const Color& color)
    : bounds(rect), color(color) {}

void RigidBody::Draw() const
{
    DrawRectangleRec(bounds, color);
}

Rectangle RigidBody::GetBounds() const
{
    return bounds;
}