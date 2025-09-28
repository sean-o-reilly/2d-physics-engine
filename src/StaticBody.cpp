#include "StaticBody.h"

const Color StaticBody::defaultColor = BLUE;

StaticBody::StaticBody(const Rectangle& rect)
    : StaticBody(rect, defaultColor)
    {}

StaticBody::StaticBody(const Rectangle& rect, const Color& color)
    : RigidBody(rect, color) {}