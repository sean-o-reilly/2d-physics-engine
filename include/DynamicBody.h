#pragma once

#include "RigidBody.h"
#include "raylib.h"

#include <nlohmann/json.hpp>

class DynamicBody : public RigidBody
{
public:
    static const std::string jsonKey;

    void Update(const float deltaTime);

    void ApplyAcceleration(const Vector2& vel);

    Vector2 GetVelocity() const;

    Rectangle GetBounds() const;

    void SetPositionY(float y);
    void SetPositionX(float x);
    
    void SetVelocityY(float y);
    void SetVelocityX(float x);

    DynamicBody(const Rectangle& rect);

    DynamicBody(const Rectangle& rect, const Color& color);

    ~DynamicBody();

    nlohmann::json ToJson() const;

    static DynamicBody FromJson(const nlohmann::json& json);

private:
    static const Color defaultColor;

    Vector2 velocity;
};