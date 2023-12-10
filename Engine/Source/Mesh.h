#pragma once
#include <vector>
#include "Globals.h"
#include "Application.h"

#define NUM_ATTRIBUTES 2

namespace tinygltf {
	class Model;
	class Mesh;
	class Primitive;
	class Accessor;
}

class Mesh {
public:
	float Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO(const tinygltf::Accessor* accessors[NUM_ATTRIBUTES], const unsigned int numAttr);
	void Render(unsigned program, const std::vector<unsigned>& textures) const;
	int GetPolygonCout() {
		if (hasEBO) return indexCount / 3;
		else return vertexCount / 3;
	}

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
