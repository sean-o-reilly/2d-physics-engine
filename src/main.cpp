#include "raylib.h"
#include "Environment.h"
#include "StaticBody.h"
#include <iostream>
#include "Utils.h"
#include <filesystem>

int main(int argc, char* argv[])
{
    const int SCREEN_WIDTH = 1920; 
    const int SCREEN_HEIGHT = 1080; 

    const std::string WIDOW_TITLE = "Testing Environment - " + GetCurrentTimeString();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WIDOW_TITLE.c_str());

    EnableCursor();

    SetTargetFPS(60);            

    const std::string ENVIRONMENT_NAME = "environment.json";

    std::filesystem::path exePath = std::filesystem::absolute(argv[0]);
    std::filesystem::path projectRoot = exePath.parent_path().parent_path().parent_path(); // from build/Debug
    std::filesystem::path saveFolder = projectRoot / "saves" / "environments";
    std::filesystem::create_directories(saveFolder.string());

    { // TEMP: Creating/destroing environment for demonstration, saving off to JSON
        Environment env;

        std::shared_ptr<StaticBody> staticBody = std::make_shared<StaticBody>(Rectangle{ 0, 0, 50, 50 });
        env.AddStaticObject(staticBody);

        if (env.SaveToJsonFile(saveFolder.string() + "/" + ENVIRONMENT_NAME)) 
        {
            std::cout << "Environment saved successfully." << std::endl;
        } 
        else 
        {
            std::cerr << "Failed to save environment." << std::endl;
        }
    }

    Environment loadedEnv = Environment::LoadFromFile(saveFolder.string() + "/" + ENVIRONMENT_NAME);

    while (!WindowShouldClose()) 
    {
        PollInputEvents();  // End of inputs for this frame

        BeginDrawing();

        ClearBackground(RAYWHITE);

        loadedEnv.Draw();

        EndDrawing();
    }

    CloseWindow(); 
    
    return 0;
}