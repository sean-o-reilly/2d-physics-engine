#pragma once
#include "raylib.h"

class EnvironmentCamera
{
public:
    EnvironmentCamera();
    EnvironmentCamera(const Camera2D &camera);

    Camera2D& Get();
    const Camera2D& Get() const;

    void Update();
    void DrawCrosshairs() const;

private:
    Camera2D camera;

    float cameraSpeed = 5.0f;
};
