#pragma once
#include "Module.h"
#include "Globals.h"
#include <GL/glew.h>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;


class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	static char* LoadShaderSource(const char* shader_file_name);
	static unsigned CompileShader(unsigned type, const char* source);
	static unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	static void RenderVBO(unsigned vbo, unsigned program);

};
