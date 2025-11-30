#include "Solver.h"

#include <vector>
#include "DynamicBody.h"
#include "StaticBody.h"

#include <fstream>
#include <filesystem>
#include <algorithm>

namespace Solver
{
    void CollisionBruteForce(Environment* environment)
    {
        auto AABB = [](const Rectangle& a, const Rectangle& b)
        {
            return a.x < b.x + b.width &&
                a.x + a.width > b.x &&
                a.y < b.y + b.height &&
                a.y + a.height > b.y;
        };

        auto getMinOverlap = [](const Rectangle& a, const Rectangle& b) -> std::pair<float, float>
        {
            float overlapLeft   = (a.x + a.width) - b.x; // the amount that A's right side cuts into B's left
            float overlapRight  = (b.x + b.width) - a.x; // the amount that A's bottom cuts into B's top
            float overlapTop    = (a.y + a.height) - b.y; // the amount that B's right cuts into A's left
            float overlapBottom = (b.y + b.height) - a.y; // the amount that B's bottom cuts into A's top

            float minX = (overlapLeft < overlapRight) ? overlapLeft : -overlapRight;
            float minY = (overlapTop < overlapBottom) ? overlapTop : -overlapBottom;

            return {minX, minY};
        };
        
        std::vector<DynamicBody>& dynamicBodies = environment->GetDynamicBodies();
        std::vector<StaticBody>& staticBodies = environment->GetStaticBodies();
        
        // Dynamic vs Dynamic
        for (size_t i = 0; i < dynamicBodies.size(); ++i)
        {
            for (size_t j = i + 1; j < dynamicBodies.size(); ++j)
            {   
                Rectangle a = dynamicBodies[i].GetBounds();
                Rectangle b = dynamicBodies[j].GetBounds();
                
                if (AABB(a, b))
                {
                    auto [minX, minY] = getMinOverlap(a, b);
                    
                    if (std::abs(minX) < std::abs(minY)) // resolve along X axis
                    {
                        // Move each half the overlap
                        dynamicBodies[i].SetPositionX(a.x - minX / 2.0f);
                        dynamicBodies[j].SetPositionX(b.x + minX / 2.0f);
                        
                        // Apply impulse-based collision with restitution
                        float systemRestitution = std::min(dynamicBodies[i].restitution, dynamicBodies[j].restitution);
                        float relativeVelocity = dynamicBodies[j].velocity.x - dynamicBodies[i].velocity.x;
                        float impulse = (1.0f + systemRestitution) * relativeVelocity / 2.0f;
                        
                        dynamicBodies[i].velocity.x += impulse;
                        dynamicBodies[j].velocity.x -= impulse;                        
                    }
                    else // resolve along Y axis
                    {
                        dynamicBodies[j].SetPositionY(b.y + minY / 2.0f);
                        dynamicBodies[i].SetPositionY(a.y - minY / 2.0f);
                        
                        // Swap velocities and apply restitution to relative velocity
                        float relativeVelocity = dynamicBodies[j].velocity.y - dynamicBodies[i].velocity.y;
                        float systemRestitution = std::min(dynamicBodies[i].restitution, dynamicBodies[j].restitution);
                        std::swap(dynamicBodies[i].velocity.y, dynamicBodies[j].velocity.y);
                        
                        dynamicBodies[i].velocity.y -= relativeVelocity * (1.0f - systemRestitution);
                        dynamicBodies[j].velocity.y += relativeVelocity * (1.0f - systemRestitution);                        
                    }
                    
                    dynamicBodies[i].TryToSleep();
                    dynamicBodies[j].TryToSleep();
                }
            }
        }
        
        // Dynamic vs Static
        for (auto& dyn : dynamicBodies)
        {
            for (auto& stat : staticBodies)
            {
                Rectangle a = dyn.GetBounds();
                Rectangle b = stat.GetBounds();

                if (AABB(a, b))
                {
                    auto [minX, minY] = getMinOverlap(a, b);

                    // Resolve along axis of least penetration
                    if (std::abs(minX) < std::abs(minY))
                    {
                        dyn.SetPositionX(a.x - minX);
                        dyn.velocity.x = -dyn.velocity.x * dyn.restitution;
                    } 
                    else
                    {
                        dyn.SetPositionY(a.y - minY);
                        dyn.velocity.y = -dyn.velocity.y * dyn.restitution;
                    }

                    dyn.TryToSleep();
                }
            }
        }
    }
}