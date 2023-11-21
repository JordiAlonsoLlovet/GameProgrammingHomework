#pragma once

#include "Globals.h"
#include "Module.h"


class ModuleTimer : Module
{
public:

    ModuleTimer();
    ~ModuleTimer();

    bool Init();
    update_status Update();
    bool CleanUp();

    double GetDeltaTime() { return deltaTime; }

private:
    const int fpsLimit = 120;
    double minDelta;
    double ptime;
    double deltaTime;

};
