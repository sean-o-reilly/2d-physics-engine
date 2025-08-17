#pragma once
#include <vector>
#include <memory>
#include "StaticBody.h"
// #include "DynamicBody.h"

struct ObjectContainer {
    std::vector<std::shared_ptr<StaticBody>> staticObjects;
    // std::vector<std::shared_ptr<DynamicBody>> dynamicObjects;
};

class Environment {
public:
    void AddStaticObject(std::shared_ptr<StaticBody> obj);

    void RemoveStaticObject(std::shared_ptr<StaticBody> obj);

    // void AddDynamicObject(std::shared_ptr<DynamicBody> obj);

    // void RemoveDynamicObject(std::shared_ptr<DynamicBody> obj);

    void Update(float dt);
    
    void Draw();
private:
    ObjectContainer objects;
};