#pragma once
#include "RigidBody.h"

#include <nlohmann/json.hpp>

class StaticBody : public RigidBody
{
public:
    static const std::string jsonKey;

    StaticBody(const Rectangle& rect);

    StaticBody(const Rectangle& rect, const Color& color);
    
    ~StaticBody();

    nlohmann::json ToJson() const;

    static StaticBody FromJson(const nlohmann::json& j);

private:
    static const Color defaultColor;
};