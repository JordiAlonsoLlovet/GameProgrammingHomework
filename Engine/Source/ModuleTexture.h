#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>

struct Metadata {
	unsigned id;
	const std::string name;
	size_t width;
	size_t height;
	size_t minMapLevel;
};
	
class ModuleTexture : public Module
{
public:

	unsigned LoadTextureFromFile(const wchar_t* tex_filename);
	Metadata GetMetadata(unsigned tex);

private:
	std::vector<Metadata> savedTextures;
};

