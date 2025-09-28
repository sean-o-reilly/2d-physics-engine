#pragma once

#include "RigidBody.h"

class StaticBody : public RigidBody
{
public:
    StaticBody(const Rectangle& rect);

    StaticBody(const Rectangle& rect, const Color& color);
    
    static const Color defaultColor;

private:
    StaticBody() = delete;

};