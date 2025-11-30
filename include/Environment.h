#pragma once

#include "raylib.h"
#include "StaticBody.h"
#include "DynamicBody.h"
#include "EnvironmentCamera.h"

#include <vector>

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

    std::vector<StaticBody>& GetStaticBodies();

    std::vector<DynamicBody>& GetDynamicBodies();

private:
    std::vector<StaticBody> staticBodies;

    std::vector<DynamicBody> dynamicBodies;

    EnvironmentCamera envCamera;

    float gravity;
};