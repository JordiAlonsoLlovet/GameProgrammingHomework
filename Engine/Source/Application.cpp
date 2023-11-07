#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleProgram.h"
#include "ModuleRenderExercise.h"
#include "debug_draw/ModuleDebugDraw.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(render = new ModuleOpenGL());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(exercice = new ModuleRenderExercice());
	modules.push_back(dd = new ModuleDebugDraw());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	ptime = clock();
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	deltaTime = (clock() - ptime) /(double) CLOCKS_PER_SEC;
	ptime = clock();
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
