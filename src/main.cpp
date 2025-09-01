#include "Engine.h"

int main()
{ 
    EnableLeakDetection();
    Engine engine;
    engine.Run();
    return 0;
}