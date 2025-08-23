#pragma once
#include "Object.h"

#include <nlohmann/json.hpp>

class StaticBody : public Object
{
public:
    static const std::string jsonKey;

    StaticBody(const Rectangle& rect);

    StaticBody(const Rectangle& rect, const Color& color);

    void Draw() const override;
    
    ~StaticBody();

    nlohmann::json ToJson() const;

    static StaticBody FromJson(const nlohmann::json& j);

private:
    static const Color defaultColor;
};