#include "Globals.h"
#include "ModuleBakerHouse.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"
#include "MathGeoLib.h"
#include "SDL.h"
#include <GL/glew.h>
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#include "tinygltf/tiny_gltf.h"

ModuleBakerHouse::ModuleBakerHouse()
{

}

// Destructor
ModuleBakerHouse::~ModuleBakerHouse()
{
}

void ModuleBakerHouse::Load(const char* assetFileName)
{
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
	}
}

// Called before render is available
bool ModuleBakerHouse::Init()
{
	float4x4 modelprojection = float4x4::FromTRS(float3(2.0f, 0.0f, -3.0f),
		float4x4::RotateZ(0),
		float3(1.0f, 1.0f, 1.0f));
	texture = ModuleTexture::LoadTextureFromFile(L"./resources/BakerHouse/Baker_House.png");
	//App->GetTexture()->LoadTextureGPU();
	char* vSource = App->GetProgram()->LoadShaderSource("../Source/VertexShaderTexture.glsl");
	char* fSource = App->GetProgram()->LoadShaderSource("../Source/FragmentShaderTexture.glsl");
	GLuint vertexShader = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vSource);
	GLuint fragmentShader = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fSource);
	program = App->GetProgram()->CreateProgram(vertexShader, fragmentShader);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &modelprojection[0][0]);
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, "./resources/BakerHouse/BakerHouse.gltf");
	if (!loadOk)
	{
		LOG("Error loading %s: %s", "BakerHouse", error.c_str());
	}
	for (const auto& srcMesh : model.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			const auto& itPos = primitive.attributes.find("POSITION");
			if (itPos != primitive.attributes.end())
			{
				const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
				SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
				SDL_assert(posAcc.componentType == GL_FLOAT);
				const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
				const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
				const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);
				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vbo);
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);
				float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
				for (size_t i = 0; i < posAcc.count; ++i)
				{
					ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
					bufferPos += posView.byteStride;
				}
				glUnmapBuffer(GL_ARRAY_BUFFER);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));

				glBindVertexArray(0);
			}

		}
	}


	return true;
}

update_status ModuleBakerHouse::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleBakerHouse::Update()
{
	float4x4 proj = App->GetCamera()->GetProjection();
	float4x4 view = App->GetCamera()->GetView();
	glBindVertexArray(vao);

	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);

	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 16, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glUseProgram(0);
	return UPDATE_CONTINUE;
}

update_status ModuleBakerHouse::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleBakerHouse::CleanUp()
{
	glDeleteProgram(program);
	return true;
}


