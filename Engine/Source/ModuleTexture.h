#pragma once
#include "Module.h"
#include "Globals.h"
	
class ModuleTexture : public Module
{
public:

	static unsigned LoadTextureFromFile(const wchar_t* tex_filename);

};

