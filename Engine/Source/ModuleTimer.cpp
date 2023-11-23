#pragma once
#include "ModuleTimer.h"
#include "MathGeoLib.h"
#include "SDL/include/SDL.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

ModuleTimer::ModuleTimer() {}
ModuleTimer::~ModuleTimer() {}

bool ModuleTimer::Init() {
	minDelta = 1000 / (long)fpsLimit;
	ptime = std::clock();
	
	longTime = std::clock();
	return true;
}
update_status ModuleTimer::Update() {
	++frameCounter;
	static Uint64 LastTime = SDL_GetPerformanceCounter();
	static Uint64 frequency = SDL_GetPerformanceFrequency();
	static double minElaps = frequency / fpsLimit;
	Uint64 ncounter = SDL_GetPerformanceCounter();
	Uint64 ElapsedTime = (ncounter - LastTime);

	deltaTime = (clock() - ptime);
	/*if (ElapsedTime < minElaps)
	{
		SDL_Delay((minElaps - ElapsedTime)*CLOCKS_PER_SEC/frequency);
		ncounter = SDL_GetPerformanceCounter();
		ElapsedTime = (ncounter - LastTime);
	}*/

	if (deltaTime < minDelta)
	{
		SDL_Delay(minDelta - deltaTime);
		deltaTime = (clock() - ptime);
	}

	ptime = clock();
	LastTime = ncounter;

	//double MSPerFrame = (((1000.0f * (double)ElapsedTime) / (double)frequency));
	//double FPS = (double)frequency / (double)ElapsedTime;
	long c = ptime - longTime;
	if (c >= 500) {
		
		fps = frameCounter * CLOCKS_PER_SEC / c;
		frameCounter = 0;
		longTime = clock();
	}
	char* t = FORMAT("FPS: %.0f", fps);
	ImGui::Text(t);
	return UPDATE_CONTINUE;
}

bool ModuleTimer::CleanUp() {
	return true;
}