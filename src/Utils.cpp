#include "Utils.h"
#include <ctime>

#include <iostream>
#include <filesystem>

Environment SelectLoadedEnvironment()
{
    std::filesystem::path exePath = std::filesystem::absolute("./");
    std::filesystem::path projectRoot = exePath.parent_path().parent_path().parent_path(); // cd backwards from build/Debug
    std::filesystem::path saveFolder = projectRoot / "saves" / "environments";
    std::filesystem::create_directories(saveFolder.string());

    std::string envName;
    std::cout << "Enter environment file name (or press Enter for empty environment): ";
    std::getline(std::cin, envName);

    Environment loadedEnv;

    if (!envName.empty())
    {
        try
        {
            loadedEnv = Environment::LoadFromFile(saveFolder.string() + "/" + envName);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to load environment: " << e.what() << std::endl;
            loadedEnv = Environment(); // Load empty environment on fail
        }
    }
    else
    {
        loadedEnv = Environment();
    }

    return loadedEnv;
}

std::string GetCurrentTimeString() 
{
    time_t now = time(nullptr);
    struct tm localTime;
    localtime_s(&localTime, &now);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &localTime);
    return timeStr;
}