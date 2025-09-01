#include "EnvironmentCamera.h"
#include "raylib.h"

#include <cmath>
#include <algorithm>

EnvironmentCamera::EnvironmentCamera()
{
    camera = {0};
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.target = {0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

EnvironmentCamera::EnvironmentCamera(const Camera2D &camera)
    : camera(camera)
{}

Camera2D &EnvironmentCamera::Get()
{
    return camera;
}

const Camera2D &EnvironmentCamera::Get() const
{
    return camera;
}

void EnvironmentCamera::Update(const float deltaTime)
{
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }; // Center the camera

    // Zoom
    targetZoom = expf(logf(targetZoom) + (GetMouseWheelMove() * zoomStep)); // Mouse wheel calculates target zoom
    targetZoom = std::clamp(targetZoom, minZoom, maxZoom);

    // Lerp will apply smooth zoom exponentially since way add by frame.
    // The camera will zoom slower as its actual zoom gets closer to its target zoom
    camera.zoom += zoomSpeed * ((targetZoom - camera.zoom) * zoomLerp) * deltaTime;

    const float cameraZoomInverse = 1.0f / camera.zoom;

    baseCameraSpeed = cameraZoomInverse * cameraZoomBoost; // Camera moves slower when zoomed in, faster when zoomed out

    // Shift to speed up
    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
    {
        currCameraSpeed = baseCameraSpeed + cameraSpeedBoost;
    }
    else
    {
        currCameraSpeed = baseCameraSpeed;
    }

    // Move
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        camera.target.x += currCameraSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        camera.target.x -= currCameraSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        camera.target.y -= currCameraSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        camera.target.y += currCameraSpeed * deltaTime;
    }
}

void EnvironmentCamera::DrawCrosshairs() const
{
    /*
        There will be adjustable settings for crosshair in the future. 
        Options to hide it, change size, etc.
    */

    int screenCenterX = GetScreenWidth() / 2; 
    int screenCenterY = GetScreenHeight() / 2;
    DrawLine(screenCenterX - 30, screenCenterY, screenCenterX + 30, screenCenterY, GREEN);
    DrawLine(screenCenterX, screenCenterY - 30, screenCenterX, screenCenterY + 30, GREEN);
}