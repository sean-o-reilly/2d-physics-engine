#include "Environment.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

Environment::Environment()
    : envCamera(), gravity(3.0f)
{}

Environment::Environment(const Environment& other)
{
    envCamera = other.envCamera;
    gravity = other.gravity;

    // Deep copy object vectors
    for (const auto& obj : other.staticObjects)
    {
        staticObjects.push_back(obj);
    }
    for (const auto& obj : other.dynamicObjects)
    {
        dynamicObjects.push_back(obj);
    }
}

Environment& Environment::operator=(const Environment& other)
{
    if (this != &other)
    {
        staticObjects.clear();
        dynamicObjects.clear();

        // Deep copy object vectors
        for (const auto& obj : other.staticObjects)
        {
            staticObjects.push_back(obj);
        }
        for (const auto& obj : other.dynamicObjects)
        {
            dynamicObjects.push_back(obj);
        }

        // NOTE: Camera is shallow copied in the assignment operator. This allows for camera position to persist in between environment resets.
        gravity = other.gravity;
    }

    return *this;
}

void Environment::AddStaticBody(const StaticBody& obj) 
{
    staticObjects.push_back(obj);
}

void Environment::AddDynamicBody(const DynamicBody& obj) 
{
    dynamicObjects.push_back(obj);
}

void Environment::ApplyGravity() 
{
    for (DynamicBody& dynamicObj : dynamicObjects) 
    {
        dynamicObj.ApplyAcceleration({/*x+=*/0, /*y+=*/ gravity});
    }
}

void Environment::CollisionBruteForce()
{
    auto AABB = [](const Rectangle& a, const Rectangle& b)
    {
        return a.x < b.x + b.width &&
               a.x + a.width > b.x &&
               a.y < b.y + b.height &&
               a.y + a.height > b.y;
    };

    // Dynamic vs Static
    for (auto& dyn : dynamicObjects)
    {
        for (auto& stat : staticObjects)
        {
            if (AABB(dyn.GetBounds(), stat.GetBounds()))
            {
                const Rectangle a = dyn.GetBounds();
                const Rectangle b = stat.GetBounds();
                float overlapLeft   = (a.x + a.width) - b.x;
                float overlapRight  = (b.x + b.width) - a.x;
                float overlapTop    = (a.y + a.height) - b.y;
                float overlapBottom = (b.y + b.height) - a.y;

                float minX = (overlapLeft < overlapRight) ? overlapLeft : -overlapRight;
                float minY = (overlapTop < overlapBottom) ? overlapTop : -overlapBottom;

                // Resolve along axis of least penetration
                if (std::abs(minX) < std::abs(minY)) {
                    dyn.SetPositionX(a.x - minX);
                    dyn.SetVelocityX(0);
                } else {
                    dyn.SetPositionY(a.y - minY);
                    dyn.SetVelocityY(0);
                }
            }
        }
    }

    // Dynamic vs Dynamic
    for (size_t i = 0; i < dynamicObjects.size(); ++i)
    {
        for (size_t j = i + 1; j < dynamicObjects.size(); ++j)
        {
            if (AABB(dynamicObjects[i].GetBounds(), dynamicObjects[j].GetBounds()))
            {
                Rectangle a = dynamicObjects[i].GetBounds();
                Rectangle b = dynamicObjects[j].GetBounds();
                float overlapLeft   = (a.x + a.width) - b.x;
                float overlapRight  = (b.x + b.width) - a.x;
                float overlapTop    = (a.y + a.height) - b.y;
                float overlapBottom = (b.y + b.height) - a.y;

                float minX = (overlapLeft < overlapRight) ? overlapLeft : -overlapRight;
                float minY = (overlapTop < overlapBottom) ? overlapTop : -overlapBottom;

                // Resolve along axis of least penetration
                if (std::abs(minX) < std::abs(minY)) {
                    // Move each half the overlap in x
                    dynamicObjects[i].SetPositionX(a.x - minX / 2.0f);
                    dynamicObjects[j].SetPositionX(b.x + minX / 2.0f);
                    dynamicObjects[i].SetVelocityX(0);
                    dynamicObjects[j].SetVelocityX(0);
                } else {
                    // Move each half the overlap in y
                    dynamicObjects[i].SetPositionY(a.y - minY / 2.0f);
                    dynamicObjects[j].SetPositionY(b.y + minY / 2.0f);
                    dynamicObjects[i].SetVelocityY(0);
                    dynamicObjects[j].SetVelocityY(0);
                }
            }
        }
    }
}

void Environment::Update(float deltaTime) 
{
    envCamera.Update(deltaTime);
    
    ApplyGravity();

    for (DynamicBody& dynamicObj : dynamicObjects) 
    {
        dynamicObj.Update(deltaTime);
    }
    
    CollisionBruteForce();
}

void Environment::Draw() const
{
    BeginMode2D(envCamera.Get());

    for (const StaticBody& staticObj : staticObjects) 
    {
        staticObj.Draw();
    }

    for (const DynamicBody& dynamicObj : dynamicObjects) 
    {
        dynamicObj.Draw();
    }

    EndMode2D();
    
    envCamera.DrawCrosshairs();
}

nlohmann::json Environment::ToJson() const
{
    nlohmann::json json;

    for (const auto& obj : staticObjects) 
    {
        json[StaticBody::jsonKey].push_back(obj.ToJson());
    }

    for (const auto& obj : dynamicObjects) 
    {
        json[DynamicBody::jsonKey].push_back(obj.ToJson());
    }

    return json;
}

Environment Environment::FromJson(const nlohmann::json& json)
{
    Environment env;
    if (json.contains(StaticBody::jsonKey.c_str())) 
    {
        for (const auto& objJson : json[StaticBody::jsonKey]) 
        {
            env.AddStaticBody(StaticBody::FromJson(objJson));
        }
    }

    if (json.contains(DynamicBody::jsonKey.c_str()))
    {
        for (const auto& objJson : json[DynamicBody::jsonKey])
        {
            env.AddDynamicBody(DynamicBody::FromJson(objJson));
        }
    }

    return env;
}

bool Environment::SaveToJsonFile(const std::string& path) const 
{
    nlohmann::json json = this->ToJson();
    std::ofstream file(path);

    if (!file.is_open()) 
    {
        return false;
    }

    file << json.dump(4);
    return true;
}

EnvironmentLoadResult Environment::LoadFromJsonFile(const std::string& path, Environment& env) 
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        return EnvironmentLoadResult::FileNotFound;
    }

    nlohmann::json json;

    try 
    {
        file >> json;
    } 
    catch (const nlohmann::json::parse_error& excpt) 
    {
        std::cerr << std::endl << excpt.what() << std::endl;
        return EnvironmentLoadResult::JsonParseError;
    }

    env = Environment::FromJson(json);

    return EnvironmentLoadResult::Success;
}