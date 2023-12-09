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
    long minDelta;
    long ptime;
    long deltaTime;
    long longTime;
    int frameCounter = 0;

};
