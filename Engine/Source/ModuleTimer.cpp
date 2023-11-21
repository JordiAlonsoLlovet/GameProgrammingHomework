#pragma once
#include "ModuleTimer.h"
#include "MathGeoLib.h"
#include "SDL/include/SDL.h"

ModuleTimer::ModuleTimer() {}
ModuleTimer::~ModuleTimer() {}

bool ModuleTimer::Init() {
	minDelta = 1000 / (double)fpsLimit;
	ptime = std::clock();
	return true;
}
update_status ModuleTimer::Update() {

	double ms = (clock() - ptime);
	deltaTime = ms / (double)CLOCKS_PER_SEC;
	if (ms < minDelta)
	{
		SDL_Delay(minDelta - ms);
		deltaTime = minDelta / (double)CLOCKS_PER_SEC;
	}
	
	ptime = clock();

	return UPDATE_CONTINUE;
}

bool ModuleTimer::CleanUp() {
	return true;
}