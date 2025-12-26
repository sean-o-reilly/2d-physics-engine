#include "Environment.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

#include "Solver.h"

Environment::Environment()
    : envCamera(), gravity(3.0f)
{}

Environment::Environment(const Environment& other)
{
    envCamera = other.envCamera;
    gravity = other.gravity;

    // Deep copy object vectors
    for (const auto& obj : other.staticBodies)
    {
        staticBodies.push_back(obj);
    }
    for (const auto& obj : other.dynamicBodies)
    {
        dynamicBodies.push_back(obj);
    }
}

Environment& Environment::operator=(const Environment& other)
{
    if (this != &other)
    {
        staticBodies.clear();
        dynamicBodies.clear();

        // Deep copy object vectors
        for (const auto& obj : other.staticBodies)
        {
            staticBodies.push_back(obj);
        }
        for (const auto& obj : other.dynamicBodies)
        {
            dynamicBodies.push_back(obj);
        }

        // HACK: Camera is shallow copied in the assignment operator. This allows for camera position to persist in between environment resets.
        gravity = other.gravity;
    }

    return *this;
}

void Environment::AddStaticBody(const StaticBody& obj) 
{
    staticBodies.push_back(obj);
}

void Environment::AddDynamicBody(const DynamicBody& obj) 
{
    dynamicBodies.push_back(obj);
}

void Environment::Update(const float deltaTime) 
{
    envCamera.Update(deltaTime);
    
    for (DynamicBody& dynamicObj : dynamicBodies) 
    {
        dynamicObj.Update(deltaTime);
        dynamicObj.ApplyAcceleration({/*x+=*/0, /*y+=*/ gravity});
    }
    
    Solver::CollisionPrecompute(this);
}

void Environment::Draw() const
{
    BeginMode2D(envCamera.Get());

    for (const StaticBody& staticObj : staticBodies) 
    {
        staticObj.Draw();
    }

    for (const DynamicBody& dynamicObj : dynamicBodies) 
    {
        dynamicObj.Draw();
    }

    EndMode2D();
    
    envCamera.DrawCrosshairs();
}

std::vector<StaticBody>& Environment::GetStaticBodies()
{
    return staticBodies;
}

std::vector<DynamicBody>& Environment::GetDynamicBodies()
{
    return dynamicBodies;
}