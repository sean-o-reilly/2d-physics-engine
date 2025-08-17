#include "Environment.h"

#include <fstream>
#include <nlohmann/json.hpp>

void Environment::AddStaticObject(std::shared_ptr<StaticBody> obj) {
    objects.staticObjects.push_back(obj);
}

void Environment::RemoveStaticObject(std::shared_ptr<StaticBody> obj) {
    auto& vec = objects.staticObjects;
    vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
}

// void Environment::AddDynamicObject(std::shared_ptr<DynamicBody> obj) {
//     objects.dynamicObjects.push_back(obj);
// }

// void Environment::RemoveDynamicObject(std::shared_ptr<DynamicBody> obj) {
//     auto& vec = objects.dynamicObjects;
//     vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
// }

void Environment::Update(float dt) {
    // for (auto& obj : objects.dynamicObjects) {
    //     if (obj) obj->Update(dt);
    // }
}

void Environment::Draw() {
    for (const std::shared_ptr<StaticBody>& obj : objects.staticObjects) {
        if (obj) obj->Draw();
    }
    // for (const std::shared_ptr<DynamicBody>& obj : objects.dynamicObjects) {
    //     if (obj) obj->Draw();
    // }
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