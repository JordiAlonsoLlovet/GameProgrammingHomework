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
	void LoadMaterials(const tinygltf::Model& srcModel, const char* dir);
	bool CleanUp();
	void Load(const char* dir, const char* assetFileName);
	void ChangeModel(const char* fileDir);
	float GetModelSize() const { return realSize; }

private:
	unsigned program;
	std::vector<Mesh*> meshes;
	std::vector<unsigned> textures;
	unsigned defTexture;
	float modelSize;
	float realSize;
};
