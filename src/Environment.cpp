#include "Environment.h"

#include <fstream>
#include <nlohmann/json.hpp>

static const Color defaultColor{0, 121, 241, 255}; // BLUE's RGBA

void Environment::AddStaticObject(std::shared_ptr<StaticBody> obj) 
{
    objects.staticObjects.push_back(obj);
}

void Environment::RemoveStaticObject(std::shared_ptr<StaticBody> obj) 
{
    auto& vec = objects.staticObjects;
    vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
}

// void Environment::AddDynamicObject(std::shared_ptr<DynamicBody> obj) {}

// void Environment::RemoveDynamicObject(std::shared_ptr<DynamicBody> obj) {}

void Environment::Update() 
{
    envCamera.Update();
}

void Environment::Draw() const
{
    BeginMode2D(envCamera.Get());

    for (const std::shared_ptr<StaticBody>& obj : objects.staticObjects) 
    {
        if (obj) 
        {
            obj->Draw();
        }
    }

    EndMode2D();
    
    envCamera.DrawCrosshairs();
}

nlohmann::json Environment::ToJson() const
{
    nlohmann::json json;

    for (const auto& obj : objects.staticObjects) 
    {
        if (obj) 
        {
            json[StaticBody::jsonKey].push_back(obj->ToJson());
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
    file >> json;

    env = Environment::FromJson(json);

    return EnvironmentLoadResult::Success;
}