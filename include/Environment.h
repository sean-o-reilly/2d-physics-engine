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

    const std::vector<StaticBody>& GetStaticBodies() const;

    const std::vector<DynamicBody>& GetDynamicBodies() const;

private:
    std::vector<StaticBody> staticBodies;

    std::vector<DynamicBody> dynamicBodies;

    EnvironmentCamera envCamera;

    float gravity;

    void ApplyGravity();

    void CollisionBruteForce();
};