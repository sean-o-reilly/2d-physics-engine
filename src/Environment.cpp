#include "Environment.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

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

void Environment::ApplyGravity() 
{
    for (DynamicBody& dynamicObj : dynamicBodies) 
    {
        dynamicObj.ApplyAcceleration({/*x+=*/0, /*y+=*/ gravity});
    }
}

void Environment::CollisionBruteForce()
{
    auto AABB = [](const Rectangle& a, const Rectangle& b)
    {
        return a.x < b.x + b.width &&
               a.x + a.width > b.x &&
               a.y < b.y + b.height &&
               a.y + a.height > b.y;
    };

    // Dynamic vs Static
    for (auto& dyn : dynamicBodies)
    {
        for (auto& stat : staticBodies)
        {
            if (AABB(dyn.GetBounds(), stat.GetBounds()))
            {
                const Rectangle a = dyn.GetBounds();
                const Rectangle b = stat.GetBounds();
                float overlapLeft   = (a.x + a.width) - b.x;
                float overlapRight  = (b.x + b.width) - a.x;
                float overlapTop    = (a.y + a.height) - b.y;
                float overlapBottom = (b.y + b.height) - a.y;

                float minX = (overlapLeft < overlapRight) ? overlapLeft : -overlapRight;
                float minY = (overlapTop < overlapBottom) ? overlapTop : -overlapBottom;

                // Resolve along axis of least penetration
                if (std::abs(minX) < std::abs(minY)) {
                    dyn.SetPositionX(a.x - minX);
                    dyn.SetVelocityX(0);
                } else {
                    dyn.SetPositionY(a.y - minY);
                    dyn.SetVelocityY(0);
                }
            }
        }
    }

    // Dynamic vs Dynamic
    for (size_t i = 0; i < dynamicBodies.size(); ++i)
    {
        for (size_t j = i + 1; j < dynamicBodies.size(); ++j)
        {
            if (AABB(dynamicBodies[i].GetBounds(), dynamicBodies[j].GetBounds()))
            {
                Rectangle a = dynamicBodies[i].GetBounds();
                Rectangle b = dynamicBodies[j].GetBounds();
                float overlapLeft   = (a.x + a.width) - b.x;
                float overlapRight  = (b.x + b.width) - a.x;
                float overlapTop    = (a.y + a.height) - b.y;
                float overlapBottom = (b.y + b.height) - a.y;

                float minX = (overlapLeft < overlapRight) ? overlapLeft : -overlapRight;
                float minY = (overlapTop < overlapBottom) ? overlapTop : -overlapBottom;

                // Resolve along axis of least penetration
                if (std::abs(minX) < std::abs(minY)) {
                    // Move each half the overlap in x
                    dynamicBodies[i].SetPositionX(a.x - minX / 2.0f);
                    dynamicBodies[j].SetPositionX(b.x + minX / 2.0f);
                    dynamicBodies[i].SetVelocityX(0);
                    dynamicBodies[j].SetVelocityX(0);
                } else {
                    // Move each half the overlap in y
                    dynamicBodies[i].SetPositionY(a.y - minY / 2.0f);
                    dynamicBodies[j].SetPositionY(b.y + minY / 2.0f);
                    dynamicBodies[i].SetVelocityY(0);
                    dynamicBodies[j].SetVelocityY(0);
                }
            }
        }
    }
}

void Environment::Update(float deltaTime) 
{
    envCamera.Update(deltaTime);
    
    ApplyGravity();

    for (DynamicBody& dynamicObj : dynamicBodies) 
    {
        dynamicObj.Update(deltaTime);
    }
    
    CollisionBruteForce();
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

const std::vector<StaticBody>& Environment::GetStaticBodies() const
{
    return staticBodies;
}

const std::vector<DynamicBody>& Environment::GetDynamicBodies() const
{
    return dynamicBodies;
}