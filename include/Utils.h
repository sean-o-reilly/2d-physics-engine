#pragma once

#include "raylib.h"
#include <string>
#include "Environment.h"
#include <filesystem>

Environment SelectLoadedEnvironment();

std::string GetCurrentTimeString();