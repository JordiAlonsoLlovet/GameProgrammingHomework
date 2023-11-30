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
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		const tinygltf::BufferView& texView = model.bufferViews[texAcc.bufferView];
		const tinygltf::Buffer& texBuffer = model.buffers[texView.buffer];
		const unsigned char* bufferTex = &(texBuffer.data[texAcc.byteOffset + texView.byteOffset]);

		vertexCount = posAcc.count;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (3 * posAcc.count + 3 * texAcc.count), nullptr, GL_STATIC_DRAW);
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		for (size_t i = 0; i < posAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += posView.byteStride;
		}
		for (size_t i = posAcc.count; i < posAcc.count+texAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferTex);
			bufferPos += posView.byteStride;
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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * indexCount));
	glBindVertexArray(0);
}

void Mesh::Render(unsigned program, const std::vector<unsigned>& textures) const
{
	glUseProgram(program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[materialIndex]);
	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, BUFFER_OFFSET(byteOffset));
}

