#include "StaticBody.h"

const Color StaticBody::defaultColor = BLUE;

StaticBody::StaticBody(const Rectangle& rect)
    : StaticBody(rect, defaultColor, defaultFriction)
    {}

StaticBody::StaticBody(const Rectangle& rect, const Color& color, const float friction)
    : RigidBody(rect, color, friction) {}