#include "RigidBody.h"

RigidBody::RigidBody(const Rectangle& rect) 
: bounds(rect) {}

RigidBody::RigidBody(const Rectangle& rect, const Color& color)
    : bounds(rect), color(color) {}
