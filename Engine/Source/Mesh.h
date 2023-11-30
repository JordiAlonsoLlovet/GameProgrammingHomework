#pragma once
#include <vector>
#include "Globals.h"
#include "Application.h"

namespace tinygltf {
	class Model;
	class Mesh;
	class Primitive;
}

class Mesh {
public:
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO();
	void Render(unsigned program, const std::vector<unsigned>& textures) const;

private:
	unsigned int vbo;
	unsigned int ebo;
	unsigned int vao;
	int indexCount;
	int vertexCount;
	int byteOffset;
	int materialIndex;
};
