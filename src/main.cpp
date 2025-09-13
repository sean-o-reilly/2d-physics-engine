#include "Engine.h"
#include "Utils.h"

int main()
{ 
    EnableLeakDetection();
    Engine engine;
    engine.Run();
    return 0;
}