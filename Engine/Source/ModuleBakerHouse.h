#pragma once
#include<vector>
#include "Module.h"
#include "Globals.h"

namespace tinygltf {
	class Accessor;
	class Model;
}

class Mesh;

class ModuleBakerHouse : public Module
{
public:
	ModuleBakerHouse();
	~ModuleBakerHouse();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	void LoadMaterials(const tinygltf::Model& srcModel);
	bool CleanUp();
	void Load(const char* assetFile);

private:
	unsigned program;
	std::vector<Mesh*> meshes;
	std::vector<unsigned> textures;
};
