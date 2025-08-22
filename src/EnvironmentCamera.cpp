#include "EnvironmentCamera.h"
#include "raylib.h"

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

void EnvironmentCamera::Update()
{
    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
    {
        cameraSpeed = 10.0f;
    }
    else
    {
        cameraSpeed = 5.0f;
    }

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        camera.target.x += cameraSpeed;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        camera.target.x -= cameraSpeed;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        camera.target.y -= cameraSpeed;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        camera.target.y += cameraSpeed;
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