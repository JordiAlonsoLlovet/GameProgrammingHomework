#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleBakerHouse : public Module
{
public:
	ModuleBakerHouse();
	~ModuleBakerHouse();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void Load(const char* assetFile);

public:
	unsigned int vbo;
	unsigned int vao;

private:
	unsigned program;
	unsigned texture;
};
