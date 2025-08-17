#include "Environment.h"

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