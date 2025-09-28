#include "Serializer.h"
#include "Environment.h"
#include "StaticBody.h"
#include "DynamicBody.h"

enum class SerializerLoadResult
{
    Success,
    FileNotFound,
    JsonParseError
};

Serializer& Serializer::GetInstance()
{
    static Serializer _serializerInstance;
    return _serializerInstance;
}

Environment Serializer::EnvironmentFromJson(const nlohmann::json& json)
{
    Environment env;
    if (json.contains(SerializerConstants::StaticBodyJsonKey)) 
    {
        for (const auto& jsonObj : json[SerializerConstants::StaticBodyJsonKey]) 
        {
            env.AddStaticBody(StaticBodyFromJson(jsonObj));
        }
    }

    if (json.contains(SerializerConstants::DynamicBodyJsonKey))
    {
        for (const auto& jsonObj : json[SerializerConstants::DynamicBodyJsonKey])
        {
            env.AddDynamicBody(DynamicBodyFromJson(jsonObj));
        }
    }

    return env;
}

StaticBody Serializer::StaticBodyFromJson(const nlohmann::json& json)
{
    Rectangle rect;
    rect.x = json.at("x").get<float>();
    rect.y = json.at("y").get<float>();
    rect.width = json.at("width").get<float>();
    rect.height = json.at("height").get<float>();

    Color color;

    if (json.contains("color"))
    {
        const auto& c = json["color"];
        color.r = static_cast<unsigned char>(c.value("r", 0));
        color.g = static_cast<unsigned char>(c.value("g", 0));
        color.b = static_cast<unsigned char>(c.value("b", 0));
        color.a = static_cast<unsigned char>(c.value("a", 255));
    }
    else
    {
        color = StaticBody::defaultColor;
    }
    
    return StaticBody(rect, color);
}

DynamicBody Serializer::DynamicBodyFromJson(const nlohmann::json& json)
{
    Rectangle rect;
    rect.x = json.at("x").get<float>();
    rect.y = json.at("y").get<float>();
    rect.width = json.at("width").get<float>();
    rect.height = json.at("height").get<float>();

    Color color; 

    if (json.contains("color"))
    {
        const auto& c = json["color"];
        color.r = static_cast<unsigned char>(c.value("r", 0));
        color.g = static_cast<unsigned char>(c.value("g", 0));
        color.b = static_cast<unsigned char>(c.value("b", 0));
        color.a = static_cast<unsigned char>(c.value("a", 255));
    }
    else 
    {
        color = DynamicBody::defaultColor;
    }

    DynamicBody body(rect, color);

    if (json.contains("velocity"))
    {
        const auto& v = json["velocity"];
        body.SetVelocityX(v.value("x", 0.0f));
        body.SetVelocityY(v.value("y", 0.0f));
    }

    return body;  
}