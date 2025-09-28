#pragma once

#include <nlohmann/json.hpp>

class Environment;
class StaticBody;
class DynamicBody;

namespace SerializerConstants
{
    inline const char* DynamicBodyJsonKey = "DynamicBody";
    inline const char* StaticBodyJsonKey = "StaticBody";
}

class Serializer
{
public:
    static Serializer& GetInstance();

    Environment EnvironmentFromJson(const nlohmann::json& json);

    StaticBody StaticBodyFromJson(const nlohmann::json& json);

    DynamicBody DynamicBodyFromJson(const nlohmann::json& json);

private:
    
    Serializer() = default;
    
    Serializer(const Serializer& other) = delete;

    Serializer& operator=(const Serializer& other) = delete;
};