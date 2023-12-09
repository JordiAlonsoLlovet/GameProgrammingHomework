#include "Mesh.h"
#include "SDL.h"
#include "MathGeoLib.h"
#include "ModuleTexture.h"
#include "tinygltf/tiny_gltf.h"
#include <GL/glew.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

float Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	materialIndex = primitive.material;
	unsigned int indAcc = 0;
	const tinygltf::Accessor* accessors[NUM_ATTRIBUTES];
	float3 max;

	const auto& itPos = primitive.attributes.find("POSITION");
	const auto& itTex = primitive.attributes.find("TEXCOORD_0");
	if (itPos != primitive.attributes.end()) {
		accessors[indAcc++] = &model.accessors[itPos->second];
		vertexCount = accessors[0]->count;
		max.x = accessors[0]->maxValues[0];
		max.y = accessors[0]->maxValues[1];
		max.z = accessors[0]->maxValues[2];
	}
	if (itTex != primitive.attributes.end())
		accessors[indAcc++] = &model.accessors[itTex->second];

	unsigned int sizeOfData = 0;
	for (unsigned int i = 0; i < indAcc; ++i)
		sizeOfData += SizeFromGlType(accessors[i]->componentType) * AttributNumElements(accessors[i]->type) * accessors[i]->count;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeOfData, nullptr, GL_STATIC_DRAW);
	char* ptr = (char*)(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	unsigned int ptrIndex = 0;
	for (unsigned int i = 0; i < indAcc; ++i) {
		const tinygltf::BufferView* bufferView = &model.bufferViews[accessors[i]->bufferView];
		const tinygltf::Buffer* buffer = &model.buffers[bufferView->buffer];
		const unsigned char* buff = &(buffer->data[accessors[i]->byteOffset + bufferView->byteOffset]);
		unsigned int attributeSize = SizeFromGlType(accessors[i]->componentType) * AttributNumElements(accessors[i]->type);
		for (unsigned int j = 0; j < accessors[i]->count; ++j) {
			//float3 test = *reinterpret_cast<const float3*>(buffers[i]);
			memcpy(&ptr[ptrIndex], buff, attributeSize);
			buff += attributeSize;
			ptrIndex += attributeSize;
		}		
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	LoadEBO(model, mesh, primitive);

	///Create VAO
	CreateVAO(accessors, indAcc);

	//This just returns the maximum distance from a vertex to the origin. It's not a good way to measure the Modl's size but will do for now.
	return max.Length();
}

void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	if (hasEBO = primitive.indices >= 0)
	{
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset +
			indView.byteOffset]);
		indexCount = indAcc.count;
		byteOffset = indAcc.byteOffset;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const uint16_t* bufferInd = reinterpret_cast<const uint16_t*>(buffer);
			for (uint16_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			const byte* bufferInd = reinterpret_cast<const byte*>(buffer);
			for (byte i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}

void Mesh::CreateVAO(const tinygltf::Accessor* accessors[NUM_ATTRIBUTES], const unsigned int numAttr)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (hasEBO) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	unsigned int offset = 0;
	for (unsigned int a = 0; a < numAttr; ++a) {
		glEnableVertexAttribArray(a);
		glVertexAttribPointer(a, AttributNumElements(accessors[a]->type), accessors[a]->componentType, GL_FALSE, 0, (void*)offset);
		offset += accessors[a]->count * AttributNumElements(accessors[a]->type) * SizeFromGlType(accessors[a]->componentType);
	}
	glBindVertexArray(0);
}

void Mesh::Render(unsigned program, const std::vector<unsigned>& textures) const
{
	glUseProgram(program);
	if (materialIndex >= 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[materialIndex]);
	}
	glBindVertexArray(vao);
	if (hasEBO) glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, BUFFER_OFFSET(byteOffset));
	else glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

unsigned int Mesh::SizeFromGlType(int glDefineType)
{
	switch (glDefineType)
	{
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		return 1;
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
	case GL_2_BYTES:
		return 2;
	case GL_3_BYTES:
		return 3;
	case GL_INT:
	case GL_UNSIGNED_INT:
	case GL_FLOAT:
	case GL_4_BYTES:
		return 4;
	case GL_DOUBLE:
		return 8;
	default:
		LOG("WARNING: Could not identify GLTypeDefine");
	}
	return 0;
}

unsigned int Mesh::AttributNumElements(int tinyDefineType)
{
	switch (tinyDefineType)
	{
	case TINYGLTF_TYPE_SCALAR:
		return 1;
	case TINYGLTF_TYPE_VEC2:
		return 2;
	case TINYGLTF_TYPE_VEC3:
		return 3;
	case TINYGLTF_TYPE_VEC4:
	case TINYGLTF_TYPE_MAT2:
		return 4;
	case TINYGLTF_TYPE_MAT3:
		return 9;
	case TINYGLTF_TYPE_MAT4:
		return 16;
	default:
		//case TINYGLTF_TYPE_VECTOR
		//case TINYGLTF_TYPE_MATRIX
		LOG("WARNING: Could not identify TinyGtfTypeDefine");
		assert(false && "WARNING: Could not identify TinyGtfTypeDefine");
	}
	return 0;
}
