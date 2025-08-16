#include "Utils.h"
#include <ctime>

std::string GetCurrentTimeString() 
{
    time_t now = time(nullptr);
    struct tm localTime;
    localtime_s(&localTime, &now);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &localTime);
    return timeStr;
}
