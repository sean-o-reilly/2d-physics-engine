#include "DynamicBody.h"
#include "raymath.h"

const std::string DynamicBody::jsonKey = "DynamicBody";
const Color DynamicBody::defaultColor = GREEN;

void DynamicBody::Update(const float deltaTime)
{
    bounds.x += velocity.x * deltaTime;
    bounds.y += velocity.y * deltaTime;
}

void DynamicBody::ApplyAcceleration(const Vector2& vel)
{
    velocity = Vector2Add(velocity, vel);
}

Vector2 DynamicBody::GetVelocity() const
{
    return velocity;
}

Rectangle DynamicBody::GetBounds() const
{
    return bounds;
}

void DynamicBody::SetPositionY(float y)
{
    bounds.y = y;
}

void DynamicBody::SetPositionX(float x)
{
    bounds.x = x;
}

void DynamicBody::SetVelocityY(float y)
{
    velocity.y = y;
}

void DynamicBody::SetVelocityX(float x)
{
    velocity.x = x;
}

DynamicBody::DynamicBody(const Rectangle& rect)
    : RigidBody(rect), velocity({0.0f, 0.0f}) {}

DynamicBody::DynamicBody(const Rectangle& rect, const Color& color)
    : RigidBody(rect, color), velocity({0.0f, 0.0f}) {}

DynamicBody::~DynamicBody() {}

nlohmann::json DynamicBody::ToJson() const
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

    json["velocity"] = { {"x", velocity.x}, {"y", velocity.y} };
    return json;
}

DynamicBody DynamicBody::FromJson(const nlohmann::json& json)
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
    DynamicBody body(rect, color);
    if (json.contains("velocity"))
    {
        const auto& v = json["velocity"];
        body.velocity.x = v.value("x", 0.0f);
        body.velocity.y = v.value("y", 0.0f);
    }
    return body;
}