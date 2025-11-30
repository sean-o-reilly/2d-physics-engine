#include "Engine.h"
#include "Utils.h"

int main()
{ 
    Utils::EnableLeakDetection();
    Engine engine;
    engine.Run();
    return 0;
}