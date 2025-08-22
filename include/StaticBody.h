#pragma once
#include "Object.h"

#include <nlohmann/json.hpp>

class StaticBody : public Object
{
public:
    StaticBody(const Rectangle& rect);

    StaticBody(const Rectangle& rect, const Color& color);

    void Draw() const override;
    
    ~StaticBody();

    nlohmann::json ToJson() const;

    static StaticBody FromJson(const nlohmann::json& j);

private:
    inline static const Color defaultColor = BLUE;
};