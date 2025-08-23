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

    inline static constexpr float cameraSpeed = 5.0f;
    inline static constexpr float cameraSpeedBoost = cameraSpeed * 1.3f;
    inline static constexpr float cameraZoomBoost = cameraSpeed; 
    float currCameraSpeed; // Applied to camera positioning every frame
    float baseCameraSpeed; // Base speed is calculated in proportion to zoom and speed boost
    
    inline static constexpr float maxZoom = 4.0f;
    inline static constexpr float minZoom = 0.1f;
    inline static constexpr float zoomStep = 0.2f;
    inline static constexpr float zoomLerp = 0.15f; // Handles smooth zooming
    float targetZoom = 1.0f; // Calculated by mouse wheel input
};