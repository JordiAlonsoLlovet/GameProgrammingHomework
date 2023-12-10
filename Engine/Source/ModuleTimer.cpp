#pragma once
#include "ModuleTimer.h"
#include "MathGeoLib.h"
#include "SDL/include/SDL.h"
#include "imgui.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <vector>

ModuleTimer::ModuleTimer() {}
ModuleTimer::~ModuleTimer() {}

bool ModuleTimer::Init() {
	ptime = std::clock();
	
	longTime = std::clock();
	return true;
}
update_status ModuleTimer::Update() {
	++frameCounter;
	static int fpsLimit = 120;
	static std::vector<float> fps_log;
	
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

	if (deltaTime < 1000 / (long)fpsLimit)
	{
		SDL_Delay(1000 / (long)fpsLimit - deltaTime);
		deltaTime = (clock() - ptime);
	}

	ptime = clock();
	LastTime = ncounter;

	//double MSPerFrame = (((1000.0f * (double)ElapsedTime) / (double)frequency));
	//double FPS = (double)frequency / (double)ElapsedTime;
	long c = ptime - longTime;
	if (c >= 500) {
		fps_log.push_back(frameCounter * CLOCKS_PER_SEC / c);
		if (fps_log.size() > 100)
			fps_log.erase(fps_log.begin());
		frameCounter = 0;
		longTime = clock();
	}
	
	if (App->GetEditor()->ShowWindow(CONFIG_W)) {
		ImGui::Begin(CONFIG_W);
		ImGui::SliderInt("Max FPS", &fpsLimit, 0, 200);
		const char* t = string_format("FPS: %.0f", fps_log.back());
		float recordFps = *std::max_element(std::begin(fps_log), std::end(fps_log));
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, t, 0.0f, recordFps, ImVec2(310, 100));
		free((void*)t);
		ImGui::End();
	}
	
	return UPDATE_CONTINUE;
}

bool ModuleTimer::CleanUp() {
	return true;
}