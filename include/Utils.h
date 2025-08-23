#pragma once

#include "Environment.h"
#include <string>

void SelectLoadedEnvironment(Environment& env);

std::string GetCurrentTimeString();

// Enables MSVC CRT memory leak detection (no-op on other compilers)
void EnableLeakDetection();