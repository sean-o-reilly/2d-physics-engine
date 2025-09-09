#pragma once

#include "raylib.h"
#include "StaticBody.h"
#include "DynamicBody.h"
#include "EnvironmentCamera.h"

#include <vector>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

enum class EnvironmentLoadResult
{
    Success,
    FileNotFound,
    JsonParseError
};

class Environment 
{
public:
    Environment();

    Environment(const Environment& other);

    Environment& operator=(const Environment& other);

    void AddStaticBody(const StaticBody& obj);

    void AddDynamicBody(const DynamicBody& obj);

    void Update(const float deltaTime);
    
    void Draw() const;

    nlohmann::json ToJson() const;

    static Environment FromJson(const nlohmann::json& j);

    bool SaveToJsonFile(const std::string& path) const;

    static EnvironmentLoadResult LoadFromJsonFile(const std::string& path, Environment& env);

private:
    std::vector<StaticBody> staticObjects;

    std::vector<DynamicBody> dynamicObjects;

    EnvironmentCamera envCamera;

    float gravity;

    void ApplyGravity();

    void CollisionBruteForce();
};