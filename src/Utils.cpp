#include "Utils.h"
    
#include <ctime>
#include <iostream>
#include <filesystem>

void SelectLoadedEnvironment(Environment& env)
{
    std::filesystem::path exePath = std::filesystem::absolute("./");
    std::filesystem::path projectRoot = exePath.parent_path().parent_path().parent_path(); // cd backwards from build/Debug
    std::filesystem::path saveFolder = projectRoot / "saves" / "environments";
    std::filesystem::create_directories(saveFolder.string());

    std::string envName;
    bool loaded = false;

    do 
    {
        std::cout << "Enter environment file name (or press Enter for empty environment): ";
        std::getline(std::cin, envName);

        if (envName.empty()) 
        {
            env = Environment();
            loaded = true;
        } 
        else 
        {
            try 
            {
                env = Environment::LoadFromFile(saveFolder.string() + "/" + envName);
                loaded = true;
            } 
            catch (const std::exception& exception) 
            {
                std::cerr << "Failed to load environment: " << exception.what() << std::endl;
            }
        }
    } while (!loaded);
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