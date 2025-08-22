#include "Object.h"

Object::Object(const Rectangle& rect) 
: bounds(rect) {}

Object::Object(const Rectangle& rect, const Color& color)
    : bounds(rect), color(color) {}