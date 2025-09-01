#include "Utils.h"
    
#include <ctime>
#include <iostream>
#include <filesystem>

bool SelectLoadedEnvironment(Environment& env)
{
    std::filesystem::path exePath = std::filesystem::absolute("./");
    std::filesystem::path projectRoot = exePath; // cd backwards from build/Debug
    std::filesystem::path saveFolder = projectRoot / "saves" / "environments";
    std::filesystem::create_directories(saveFolder.string());

    std::string envName;
    std::string path;
    EnvironmentLoadResult loadResult = EnvironmentLoadResult::FileNotFound;

    do 
    {
        std::cout << "Enter environment file name (or press Enter to quit): ";
        std::getline(std::cin, envName);

        if (envName.empty()) 
        {
            return false;
        } 

        path = saveFolder.string() + "\\" + envName + ".json";
        
        loadResult = Environment::LoadFromJsonFile(path, env);

        if (loadResult == EnvironmentLoadResult::FileNotFound) 
        {
            std::cout << "Could not find environment file to load from  at path'" << path << "'. Please try again." << std::endl;
        }
        else if (loadResult == EnvironmentLoadResult::JsonParseError) 
        {
            std::cout << "ERROR: Json parse error while loading environment at path '" << path << "'. Please check json file for syntax errors." << std::endl << std::endl;
        }

    } while (loadResult != EnvironmentLoadResult::Success);

    return true;
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

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
void EnableLeakDetection() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
}
#else
void EnableLeakDetection() {} // Do nothing on non-MSVC compilers
#endif