#include "Solver.h"

#include <vector>
#include <algorithm>
#include "DynamicBody.h"
#include "StaticBody.h"

namespace Solver
{
    namespace 
    {
        bool AABB(const Rectangle& a, const Rectangle& b)
        {
            return a.x < b.x + b.width &&
                a.x + a.width > b.x &&
                a.y < b.y + b.height &&
                a.y + a.height > b.y;
        }
        
        std::pair<float, float> getMinOverlap(const Rectangle& a, const Rectangle& b)
        {
            float overlapLeft   = (a.x + a.width) - b.x;
            float overlapRight  = (b.x + b.width) - a.x;
            float overlapTop    = (a.y + a.height) - b.y;
            float overlapBottom = (b.y + b.height) - a.y;
    
            float minX = (overlapLeft < overlapRight) ? overlapLeft : -overlapRight;
            float minY = (overlapTop < overlapBottom) ? overlapTop : -overlapBottom;
    
            return {minX, minY};
        }
        
        float GetFrictionCoefficientMultiplier(const float a, const float b)
        {
            static constexpr float frictionWeight = 10.0f;
            return ((a + b) / 2.0f) * frictionWeight;
        }
    }

    void CollisionPrecompute(Environment* environment)
    {
        static constexpr float frictionClamp = 0.05f;

        std::vector<DynamicBody>& dynamicBodies = environment->GetDynamicBodies();
        std::vector<StaticBody>& staticBodies = environment->GetStaticBodies();

        const size_t numDynamic = dynamicBodies.size();
        const size_t numStatic = staticBodies.size();

        // Pre-compute all data once, static vectors to avoid heap allocations per frame
        static std::vector<Rectangle> dynBounds;
        static std::vector<float> dynFriction;
        static std::vector<float> dynRestitution;
        
        dynBounds.resize(numDynamic);
        dynFriction.resize(numDynamic);
        dynRestitution.resize(numDynamic);
        
        for (size_t i = 0; i < numDynamic; ++i)
        {
            dynBounds[i] = dynamicBodies[i].GetBounds();
            dynFriction[i] = dynamicBodies[i].GetFriction();
            dynRestitution[i] = dynamicBodies[i].restitution;
        }
        
        static std::vector<Rectangle> statBounds;
        static std::vector<float> statFriction;
        statBounds.resize(numStatic);
        statFriction.resize(numStatic);

        for (size_t i = 0; i < numStatic; ++i)
        {
            statBounds[i] = staticBodies[i].GetBounds();
            statFriction[i] = staticBodies[i].GetFriction();
        }

        // Dynamic vs Dynamic
        for (size_t i = 0; i < numDynamic; ++i)
        {
            for (size_t j = i + 1; j < numDynamic; ++j)
            {
                const Rectangle& aBounds = dynBounds[i];
                const Rectangle& bBounds = dynBounds[j];

                if (AABB(aBounds, bBounds))
                {
                    DynamicBody& a = dynamicBodies[i];
                    DynamicBody& b = dynamicBodies[j];

                    auto [minX, minY] = getMinOverlap(aBounds, bBounds);

                    const float frictionMultiplier = GetFrictionCoefficientMultiplier(dynFriction[i], dynFriction[j]);

                    if (std::abs(minX) < std::abs(minY))
                    {
                        a.SetPositionX(aBounds.x - minX / 2.0f);
                        b.SetPositionX(bBounds.x + minX / 2.0f);

                        float relativeTangentVel = b.velocity.y - a.velocity.y;
                        float frictionImpulse = relativeTangentVel * frictionMultiplier;

                        float maxFriction = std::abs(relativeTangentVel) * frictionClamp;
                        frictionImpulse = std::clamp(frictionImpulse, -maxFriction, maxFriction);

                        float systemRestitution = std::min(dynRestitution[i], dynRestitution[j]);
                        float relativeVelocity = b.velocity.x - a.velocity.x;
                        float impulse = (1.0f + systemRestitution) * relativeVelocity / 2.0f;

                        a.velocity.x += impulse;
                        b.velocity.x -= impulse;

                        a.velocity.y += frictionImpulse / 2.0f;
                        b.velocity.y -= frictionImpulse / 2.0f;
                    }
                    else
                    {
                        a.SetPositionY(aBounds.y - minY / 2.0f);
                        b.SetPositionY(bBounds.y + minY / 2.0f);

                        float relativeTangentVel = b.velocity.x - a.velocity.x;
                        float frictionImpulse = relativeTangentVel * frictionMultiplier;

                        float maxFriction = std::abs(relativeTangentVel) * frictionClamp;
                        frictionImpulse = std::clamp(frictionImpulse, -maxFriction, maxFriction);

                        float relativeVelocity = b.velocity.y - a.velocity.y;
                        float systemRestitution = std::min(dynRestitution[i], dynRestitution[j]);
                        std::swap(a.velocity.y, b.velocity.y);

                        a.velocity.y -= relativeVelocity * (1.0f - systemRestitution);
                        b.velocity.y += relativeVelocity * (1.0f - systemRestitution);

                        a.velocity.x += frictionImpulse / 2.0f;
                        b.velocity.x -= frictionImpulse / 2.0f;
                    }

                    // Sync precomputed data
                    dynBounds[i] = a.GetBounds();
                    dynBounds[j] = b.GetBounds();
                }
            }
        }

        // Dynamic vs Static
        for (size_t i = 0; i < numDynamic; ++i)
        {
            for (size_t j = 0; j < numStatic; ++j)
            {
                const Rectangle& dBounds = dynBounds[i];
                const Rectangle& sBounds = statBounds[j];

                if (AABB(dBounds, sBounds))
                {
                    DynamicBody& dyn = dynamicBodies[i];

                    const float frictionMultiplier = GetFrictionCoefficientMultiplier(dynFriction[i], statFriction[j]);

                    auto [minX, minY] = getMinOverlap(dBounds, sBounds);

                    if (std::abs(minX) < std::abs(minY))
                    {
                        dyn.SetPositionX(dBounds.x - minX);

                        float tangentVel = dyn.velocity.y;
                        float frictionImpulse = tangentVel * frictionMultiplier;

                        float maxFriction = std::abs(tangentVel) * frictionClamp;
                        frictionImpulse = std::clamp(frictionImpulse, -maxFriction, maxFriction);

                        dyn.velocity.x = -dyn.velocity.x * dynRestitution[i];
                        dyn.velocity.y -= frictionImpulse;
                    }
                    else
                    {
                        dyn.SetPositionY(dBounds.y - minY);

                        float tangentVel = dyn.velocity.x;
                        float frictionImpulse = tangentVel * frictionMultiplier;

                        float maxFriction = std::abs(tangentVel) * frictionClamp;
                        frictionImpulse = std::clamp(frictionImpulse, -maxFriction, maxFriction);

                        dyn.velocity.y = -dyn.velocity.y * dynRestitution[i];
                        dyn.velocity.x -= frictionImpulse;
                    }
                }
            }
        }
    }
}
