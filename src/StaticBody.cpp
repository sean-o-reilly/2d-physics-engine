#include "StaticBody.h"

void StaticBody::Draw() const
{
    DrawRectangleRec(bounds, BLUE);
}

StaticBody::~StaticBody() = default;

StaticBody::StaticBody(const Rectangle& rect)
    : Object(rect) {}