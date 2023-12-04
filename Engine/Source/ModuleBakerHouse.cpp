#include "Globals.h"
#include "ModuleBakerHouse.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"
#include "MathGeoLib.h"
#include "SDL.h"
#include <GL/glew.h>
#include "Mesh.h"
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
	static tinygltf::Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", "BakerHouse", error.c_str());
	}
	for (const auto& srcMesh : model.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			Mesh* mesh = new Mesh;
			mesh->Load(model, srcMesh, primitive);
			mesh->LoadEBO(model, srcMesh, primitive);
			mesh->CreateVAO();
			meshes.push_back(mesh);
		}
	}
	LoadMaterials(model);
}

// Called before render is available
bool ModuleBakerHouse::Init()
{
	float4x4 modelprojection = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f),
		float4x4::RotateZ(0),
		float3(1.0f, 1.0f, 1.0f));
	//texture = ModuleTexture::LoadTextureFromFile(L"./resources/BakerHouse/Baker_House.png");
	//App->GetTexture()->LoadTextureGPU();
	char* vSource = App->GetProgram()->LoadShaderSource("../Source/VertexShaderTexture.glsl");
	char* fSource = App->GetProgram()->LoadShaderSource("../Source/FragmentShaderTexture.glsl");
	GLuint vertexShader = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vSource);
	GLuint fragmentShader = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fSource);
	program = App->GetProgram()->CreateProgram(vertexShader, fragmentShader);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &modelprojection[0][0]);

	//Load Model
	Load("./resources/BakerHouse/BakerHouse.gltf");

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

	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);

	//glUniform1i(glGetUniformLocation(program, "mytexture"), 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glDrawElements(GL_TRIANGLES, indexAccessor->count, GL_UNSIGNED_INT, BUFFER_OFFSET(indexAccessor->byteOffset));
	for (Mesh* mesh : meshes) {
		mesh->Render(program, textures);
	}

	glBindVertexArray(0);
	glUseProgram(0);
	return UPDATE_CONTINUE;
}

update_status ModuleBakerHouse::PostUpdate()
{
	return UPDATE_CONTINUE;
}

void ModuleBakerHouse::LoadMaterials(const tinygltf::Model& srcModel)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			std::wstring widestr = std::wstring(image.uri.begin(), image.uri.end());
			widestr.insert(0, L"./resources/BakerHouse/");
			textureId = (ModuleTexture::LoadTextureFromFile(widestr.c_str()));
		}
		textures.push_back(textureId);
	}
}

// Called before quitting
bool ModuleBakerHouse::CleanUp()
{
	glDeleteProgram(program);
	return true;
}


