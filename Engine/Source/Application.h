#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleCamera;
class ModuleEditor;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleRenderExercice;
class ModuleDebugDraw;
class ModuleTexture;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput*  GetInput() { return input; }
    ModuleCamera* GetCamera() { return camera; }
    ModuleProgram* GetProgram() { return program; }
    ModuleDebugDraw* GetDD() { return dd; }
    ModuleTexture* GetTexture() { return texture; }
    double GetDeltaTime() { return deltaTime; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleProgram* program = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleRenderExercice* exercice = nullptr;
    ModuleDebugDraw* dd = nullptr;
    ModuleTexture* texture = nullptr;

    std::list<Module*> modules;
    double ptime;
    double deltaTime;

};

extern Application* App;
