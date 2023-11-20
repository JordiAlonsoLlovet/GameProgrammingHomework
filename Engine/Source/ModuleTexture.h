#pragma once
#include "Module.h"
#include "Globals.h"
#include "DirectXTex.h"

class ModuleTexture : public Module
{
public:
	bool Init();
	bool CleanUp();

	HRESULT LoadTextureFromFile(LPCWSTR tex_filename);
	HRESULT LoadTextureGPU();
	void RenderTex();

	private:
		unsigned int textures;
		DirectX::TexMetadata imageMetadata;
		DirectX::ScratchImage* image;
};