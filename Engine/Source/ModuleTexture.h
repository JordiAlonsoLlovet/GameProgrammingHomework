#pragma once
#include "Module.h"
#include "Globals.h"

namespace DirectX {
	class TexMetadata;
	class ScratchImage;
}
	
class ModuleTexture : public Module
{
public:
	ModuleTexture();

	// Destructor
	virtual ~ModuleTexture();
	bool Init();
	bool CleanUp();

	HRESULT LoadTextureFromFile(LPCWSTR tex_filename);
	HRESULT LoadTextureGPU();
	void RenderTex();

private:
	unsigned int textures;
	DirectX::TexMetadata* imageMetadata;
	DirectX::ScratchImage* image;
};

