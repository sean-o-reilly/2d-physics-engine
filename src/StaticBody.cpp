#include "StaticBody.h"

#include <nlohmann/json.hpp>

void StaticBody::Draw() const
{
    DrawRectangleRec(bounds, BLUE);
}

StaticBody::~StaticBody() = default;

StaticBody::StaticBody(const Rectangle& rect)
    : Object(rect) {}

nlohmann::json StaticBody::ToJson() const 
{
    nlohmann::json json;
    json["x"] = bounds.x;
    json["y"] = bounds.y;
    json["width"] = bounds.width;
    json["height"] = bounds.height;

    return json;
}

StaticBody StaticBody::FromJson(const nlohmann::json& json) 
{
    Rectangle rect;
    rect.x = json.at("x").get<float>();
    rect.y = json.at("y").get<float>();
    rect.width = json.at("width").get<float>();
    rect.height = json.at("height").get<float>();
    
    return StaticBody(rect);
}