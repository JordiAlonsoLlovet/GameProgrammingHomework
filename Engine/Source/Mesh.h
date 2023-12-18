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
	~Mesh();
	float Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render(unsigned program, const std::vector<unsigned>& textures) const;
	int GetPolygonCout() const {
		if (hasEBO) return indexCount / 3;
		else return vertexCount / 3;
	}

	static unsigned int SizeFromGlType(int glDefineType);
	static unsigned int AttributNumElements(int tinyDefineType);

private:
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO(const tinygltf::Accessor* accessors[NUM_ATTRIBUTES], const unsigned int numAttr);
	unsigned int vbo = 0;
	unsigned int ebo = 0;
	unsigned int vao = 0;
	bool hasEBO = false;
	int indexCount = 0;
	int vertexCount = 0;
	int byteOffset = 0;
	int materialIndex = 0;
};
