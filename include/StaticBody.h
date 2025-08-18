#pragma once
#include "Object.h"

#include <nlohmann/json.hpp>

class StaticBody : public Object
{
public:
    StaticBody(const Rectangle& rect);
    
    void Draw() const override;
    
    ~StaticBody();

    // Json Serialization
    nlohmann::json ToJson() const;

    static StaticBody FromJson(const nlohmann::json& j);
};