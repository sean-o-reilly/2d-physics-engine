#include "Environment.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

void Environment::AddStaticObject(std::shared_ptr<StaticBody> obj) 
{
    staticObjects.push_back(obj);
}

void Environment::RemoveStaticObject(std::shared_ptr<StaticBody> obj) 
{
    staticObjects.erase(std::remove(staticObjects.begin(), staticObjects.end(), obj), staticObjects.end());
}

void Environment::AddDynamicObject(std::shared_ptr<DynamicBody> obj) 
{
    dynamicObjects.push_back(obj);
}

void Environment::RemoveDynamicObject(std::shared_ptr<DynamicBody> obj) 
{
    dynamicObjects.erase(std::remove(dynamicObjects.begin(), dynamicObjects.end(), obj), dynamicObjects.end());
}

void Environment::ApplyGravity() 
{
    for (const std::shared_ptr<DynamicBody>& dynamicObj : dynamicObjects) 
    {
        if (dynamicObj) 
        {
            dynamicObj->ApplyAcceleration({/*x+=*/0, /*y+=*/ gravity});
        }
    }
}

void Environment::CollisionBruteForce()
{
    std::function<bool(const Rectangle&, const Rectangle&)> checkCollision = [](const Rectangle& a, const Rectangle& b)
    {
        return a.x < b.x + b.width &&
               a.x + a.width > b.x &&
               a.y < b.y + b.height &&
               a.y + a.height > b.y;
    };

    for (auto& dyn : dynamicObjects) // Dynamic vs Static
    {
        for (auto& stat : staticObjects)
        {
            if (dyn && stat && checkCollision(dyn->GetBounds(), stat->GetBounds()))
            {
                // Simple resolution: move dyn up to the top of stat and zero y velocity
                dyn->SetPositionY(stat->GetBounds().y - dyn->GetBounds().height);
                dyn->SetVelocityY(0);
            }
        }
    }

    for (size_t i = 0; i < dynamicObjects.size(); ++i) // Dynamic vs Dynamic
    {
        for (size_t j = i + 1; j < dynamicObjects.size(); ++j)
        {
            if (dynamicObjects[i] && dynamicObjects[j] &&
                checkCollision(dynamicObjects[i]->GetBounds(), dynamicObjects[j]->GetBounds()))
            {
                // Move one up, one down
                if (dynamicObjects[i]->GetBounds().y < dynamicObjects[j]->GetBounds().y)
                {
                    dynamicObjects[i]->SetPositionY(dynamicObjects[j]->GetBounds().y - dynamicObjects[i]->GetBounds().height);
                    dynamicObjects[i]->SetVelocityY(0);
                }
                else
                {
                    dynamicObjects[j]->SetPositionY(dynamicObjects[i]->GetBounds().y - dynamicObjects[j]->GetBounds().height);
                    dynamicObjects[j]->SetVelocityY(0);
                }
            }
        }
    }
}

void Environment::Update() 
{
    envCamera.Update();
    
    ApplyGravity();

    for (const std::shared_ptr<DynamicBody>& dynamicObj : dynamicObjects) 
    {
        if (dynamicObj) 
        {
            dynamicObj->Update();
        }
    }
    
    CollisionBruteForce();
}

void Environment::Draw() const
{
    BeginMode2D(envCamera.Get());

    for (const std::shared_ptr<StaticBody>& staticObj : staticObjects) 
    {
        if (staticObj) 
        {
            staticObj->Draw();
        }
    }

    for (const std::shared_ptr<DynamicBody>& dynamicObj : dynamicObjects) 
    {
        if (dynamicObj) 
        {
            dynamicObj->Draw();
        }
    }

    EndMode2D();
    
    envCamera.DrawCrosshairs();
}

nlohmann::json Environment::ToJson() const
{
    nlohmann::json json;

    for (const auto& obj : staticObjects) 
    {
        if (obj) 
        {
            json[StaticBody::jsonKey].push_back(obj->ToJson());
        }
    }

    for (const auto& obj : dynamicObjects) 
    {
        if (obj) 
        {
            json[DynamicBody::jsonKey].push_back(obj->ToJson());
        }
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
            env.AddStaticObject(std::make_shared<StaticBody>(StaticBody::FromJson(objJson)));
        }
    }

    if (json.contains(DynamicBody::jsonKey.c_str()))
    {
        for (const auto& objJson : json[DynamicBody::jsonKey])
        {
            env.AddDynamicObject(std::make_shared<DynamicBody>(DynamicBody::FromJson(objJson)));
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