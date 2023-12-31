#pragma once
#include "Module.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRenderExercice : public Module
{
public:
	ModuleRenderExercice();
	~ModuleRenderExercice();

	bool Init();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	unsigned int vbo;
	unsigned int vao;

private:
	unsigned program;
	unsigned int texture;
};
