#pragma once
#include "Globals.h"
#include "Module.h"


class ModuleTimer : public Module
{
public:

    ModuleTimer();
    ~ModuleTimer();

    bool Init();
    update_status Update();
    bool CleanUp();

    long GetDeltaTime() { return deltaTime; }

private:
    const int fpsLimit = 120;
    float fps = 0.0f;
    long minDelta;
    long ptime;
    long deltaTime;
    long longTime;
    int frameCounter = 0;

};
