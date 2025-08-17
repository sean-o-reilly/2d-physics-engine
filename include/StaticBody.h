#pragma once
#include "Object.h"

class StaticBody : public Object
{
public:
    StaticBody(const Rectangle& rect);
    void Draw() const override;
    ~StaticBody();
};
