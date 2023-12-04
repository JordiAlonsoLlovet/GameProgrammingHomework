#include "Globals.h"
#include "ModuleBakerHouse.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"
#include "MathGeoLib.h"
#include "SDL.h"
#include <GL/glew.h>
#include "imgui.h"
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

void ModuleBakerHouse::Load(const char* dir, const char* assetFileName)
{
	tinygltf::TinyGLTF gltfContext;
	static tinygltf::Model model;
	std::string error, warning;
	std::string sDir = dir;
	std::string sAsset = assetFileName;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, sDir + sAsset);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
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
	LoadMaterials(model, dir);
}

// Called before render is available
bool ModuleBakerHouse::Init()
{
	
	//texture = ModuleTexture::LoadTextureFromFile(L"./resources/BakerHouse/Baker_House.png");
	//App->GetTexture()->LoadTextureGPU();
	char* vSource = App->GetProgram()->LoadShaderSource("./shaders/VertexShaderTexture.glsl");
	char* fSource = App->GetProgram()->LoadShaderSource("./shaders/FragmentShaderTexture.glsl");
	GLuint vertexShader = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vSource);
	GLuint fragmentShader = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fSource);
	program = App->GetProgram()->CreateProgram(vertexShader, fragmentShader);

	//Load Model
	Load("./resources/Duck/", "Duck.gltf");

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
	static float scale = 1.0f;
	static float rotateX = 0.0f;
	static float rotateY = 0.0f;
	static float rotateZ = 0.0f;

	/*********** ImGUI ************/
	ImGui::Begin("Model Configuration");
	ImGui::DragFloat("Scale", &scale, 0.005f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::SliderFloat("RotateX", &rotateX, 0, 360);
	ImGui::SliderFloat("RotateY", &rotateY, 0, 360);
	ImGui::SliderFloat("RotateZ", &rotateZ, 0, 360);
	ImGui::End();
	/*********** End ImGUI ************/
	float4x4 modelprojection = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f),
		float4x4::RotateZ(rotateZ * pi / 180) * float4x4::RotateX(rotateX * pi / 180) * float4x4::RotateY(rotateY * pi / 180),
		float3(scale, scale, scale));

	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &modelprojection[0][0]);

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

void ModuleBakerHouse::LoadMaterials(const tinygltf::Model& srcModel, const char* dir) 
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			std::string uri = dir + image.uri;
			std::wstring widestr = std::wstring(uri.begin(), uri.end());
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


