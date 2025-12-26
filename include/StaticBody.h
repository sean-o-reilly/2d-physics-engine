#pragma once

#include "RigidBody.h"

class StaticBody : public RigidBody
{
public:
    StaticBody(const Rectangle& rect);

    StaticBody(const Rectangle& rect, const Color& color, const float friction);
    
    static const Color defaultColor;

private:
    StaticBody() = delete;

};