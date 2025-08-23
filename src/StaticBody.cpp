#include "StaticBody.h"

#include <nlohmann/json.hpp>

const std::string StaticBody::jsonKey = "StaticBody";
const Color StaticBody::defaultColor = BLUE;

void StaticBody::Draw() const
{
    DrawRectangleRec(bounds, color);
}

StaticBody::StaticBody(const Rectangle& rect)
    : StaticBody(rect, defaultColor)
    {}

StaticBody::StaticBody(const Rectangle& rect, const Color& color)
    : RigidBody(rect, color) {}

StaticBody::~StaticBody() = default;

nlohmann::json StaticBody::ToJson() const 
{
    nlohmann::json json;
    json["x"] = bounds.x;
    json["y"] = bounds.y;
    json["width"] = bounds.width;
    json["height"] = bounds.height;

    json["color"] = 
    {
        {"r", color.r},
        {"g", color.g},
        {"b", color.b},
        {"a", color.a}
    };
    return json;
}

StaticBody StaticBody::FromJson(const nlohmann::json& json) 
{
    Rectangle rect;
    rect.x = json.at("x").get<float>();
    rect.y = json.at("y").get<float>();
    rect.width = json.at("width").get<float>();
    rect.height = json.at("height").get<float>();

    Color color = defaultColor;
    if (json.contains("color"))
    {
        const auto& c = json["color"];
        color.r = static_cast<unsigned char>(c.value("r", 0));
        color.g = static_cast<unsigned char>(c.value("g", 0));
        color.b = static_cast<unsigned char>(c.value("b", 0));
        color.a = static_cast<unsigned char>(c.value("a", 255));
    }
    return StaticBody(rect, color);
}