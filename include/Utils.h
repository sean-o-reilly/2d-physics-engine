#pragma once

#include "Environment.h"
#include <string>

namespace Utils
{
    enum class EnvironmentLoadResult
    {
        Success,
        FileNotFound,
        JsonParseError
    };

    EnvironmentLoadResult LoadEnvironmentFromJsonFile(const std::string& path, Environment& env);

    bool SelectLoadedEnvironment(Environment& env);

    std::string GetCurrentTimeString();

    // Enables MSVC CRT memory leak detection
    void EnableLeakDetection();
}