#include "Environment.h"

#include <fstream>
#include <nlohmann/json.hpp>

Environment::Environment() 
{
    camera = { 0 };
    camera.offset = { 0, 0 };
    camera.target = { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void Environment::AddStaticObject(std::shared_ptr<StaticBody> obj) {
    objects.staticObjects.push_back(obj);
}

void Environment::RemoveStaticObject(std::shared_ptr<StaticBody> obj) {
    auto& vec = objects.staticObjects;
    vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
}

// void Environment::AddDynamicObject(std::shared_ptr<DynamicBody> obj) {}

// void Environment::RemoveDynamicObject(std::shared_ptr<DynamicBody> obj) {}

void Environment::Update() {
    // TODO: update dynamic objects

    UpdateCamera();
}

void Environment::Draw() {
    Update();

    BeginMode2D(camera);

    // temp crosshairs
    
    // TODO: wrap in draw objects call
    for (const std::shared_ptr<StaticBody>& obj : objects.staticObjects) {
        if (obj) obj->Draw();
    }
    // TODO: draw dynamic objects

    // TODO: crosshairs aren't drawing in here? Looks like they were drawing at top of screen.

    EndMode2D();
}

nlohmann::json Environment::ToJson() const
{
    nlohmann::json json;

    for (const auto& obj : objects.staticObjects) 
    {
        if (obj) 
        {
            json["staticObjects"].push_back(obj->ToJson());
        }
    }
    return json;
}

Environment Environment::FromJson(const nlohmann::json& json)
{
    Environment env;
    if (json.contains("staticObjects")) {
        for (const auto& objJson : json["staticObjects"]) {
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

    file << json.dump(4); // Pretty print with 4-space indentation
    return true;
}

Environment Environment::LoadFromFile(const std::string& path) 
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file"); // TODO: would rather this return an enum than crash
    }

    nlohmann::json json;
    file >> json;

    return Environment::FromJson(json);
}

void Environment::UpdateCamera()
{
    // temp controls
    if (IsKeyDown(KEY_RIGHT))
    {
        camera.target.x += 2;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        camera.target.x -= 2;
    }
    else if (IsKeyDown(KEY_UP))
    {
        camera.target.y -= 2;
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        camera.target.y += 2;
    }
}