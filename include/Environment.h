#pragma once
#include <vector>
#include <memory>
#include "StaticBody.h"
// #include "DynamicBody.h"

#include <string>
#include <nlohmann/json.hpp>

struct ObjectContainer {
    std::vector<std::shared_ptr<StaticBody>> staticObjects;
    // std::vector<std::shared_ptr<DynamicBody>> dynamicObjects;
};

class Environment {
public:

    // Object methods
    void AddStaticObject(std::shared_ptr<StaticBody> obj);

    void RemoveStaticObject(std::shared_ptr<StaticBody> obj);

    // void AddDynamicObject(std::shared_ptr<DynamicBody> obj);

    // void RemoveDynamicObject(std::shared_ptr<DynamicBody> obj);

    void Update(float dt);
    
    void Draw();

    // Json serialization methods
    nlohmann::json ToJson() const;

    static Environment FromJson(const nlohmann::json& j);

    bool SaveToJsonFile(const std::string& path) const;
    
    static Environment LoadFromFile(const std::string& path);

private:
    ObjectContainer objects;
};