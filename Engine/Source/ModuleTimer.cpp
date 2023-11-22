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
	deltaTime = (clock() - ptime);
	if (deltaTime < minDelta)
	{
		SDL_Delay(minDelta - deltaTime);
		deltaTime = (clock() - ptime);
	}
	
	ptime = clock();
	if (frameCounter >= 100) {
		long c = clock() - longTime;
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