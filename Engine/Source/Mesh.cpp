#include "Mesh.h"
#include <GL/glew.h>
#include "SDL.h"
#include "MathGeoLib.h"
#include "ModuleTexture.h"
#include "tinygltf/tiny_gltf.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	materialIndex = primitive.material;
	const auto& itPos = primitive.attributes.find("POSITION");
	const auto& itTex = primitive.attributes.find("TEXCOORD_0");
	if (itPos != primitive.attributes.end())
	{
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		const tinygltf::Accessor& texAcc = model.accessors[itTex->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		SDL_assert(texAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(texAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		const tinygltf::BufferView& texView = model.bufferViews[texAcc.bufferView];
		const tinygltf::Buffer& texBuffer = model.buffers[texView.buffer];
		const unsigned char* bufferTex = &(texBuffer.data[texAcc.byteOffset + texView.byteOffset]);

		vertexCount = posAcc.count;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		int sizeOfData = sizeof(float) * (3 * posAcc.count + 2 * texAcc.count);
		glBufferData(GL_ARRAY_BUFFER, sizeOfData, nullptr, GL_STATIC_DRAW);
		int a = sizeof(float);
		int b = sizeof(char);
		char* ptr = reinterpret_cast<char*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		//memcpy(&ptr[0], bufferPos, 12* posAcc.count);
		//memcpy(&ptr[12 * posAcc.count], bufferTex, texView.byteStride * texAcc.count);
		for (size_t i = 0; i < posAcc.count * 12; i+= 12)
		{
			//ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			memcpy(&ptr[i], bufferPos, 12);
			bufferPos += 12;
		}
		for (size_t i = posAcc.count * 12; i < sizeOfData; i+= 8)
		{
			//ptr[i] = *reinterpret_cast<const float3*>(bufferTex);
			memcpy(&ptr[i], bufferTex, 8);
			bufferTex += 8;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	
	}
}

void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	if (primitive.indices >= 0)
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

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));
	glBindVertexArray(0);
}

void Mesh::Render(unsigned program, const std::vector<unsigned>& textures) const
{
	glUseProgram(program);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[materialIndex]);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, BUFFER_OFFSET(byteOffset));
}

