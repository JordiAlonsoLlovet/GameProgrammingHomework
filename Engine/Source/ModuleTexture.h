#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleTexture : public Module
{
public:
	bool Init();
	update_status Update();
	bool CleanUp();

	HRESULT LoadTextureFromFile(LPCWSTR tex_filename);
	private:
		GLuint* textures;
		DirectX::TexMetadata imageMetadata;
		DirectX::ScratchImage* image;
};