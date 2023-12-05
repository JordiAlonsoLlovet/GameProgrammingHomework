#pragma once
#include <vector>
#include "Globals.h"
#include "Application.h"

namespace tinygltf {
	class Model;
	class Mesh;
	class Primitive;
	class Accessor;
}

class Mesh {
public:
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO(const tinygltf::Accessor* accessors, const unsigned int numAttr);
	void Render(unsigned program, const std::vector<unsigned>& textures) const;

	static unsigned int SizeFromGlType(int glDefineType);
	static unsigned int AttributNumElements(int tinyDefineType);

private:
	unsigned int vbo;
	unsigned int ebo;
	unsigned int vao;
	bool hasEBO;
	int indexCount;
	int vertexCount;
	int byteOffset;
	int materialIndex;
};
