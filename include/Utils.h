#pragma once

#include "Environment.h"
#include <string>

bool SelectLoadedEnvironment(Environment& env);

std::string GetCurrentTimeString();

// Enables MSVC CRT memory leak detection
void EnableLeakDetection();