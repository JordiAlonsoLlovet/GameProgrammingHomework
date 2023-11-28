#include "Globals.h"
#include "ModuleBakerHouse.h"
#include "Application.h"
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
	texture = ModuleTexture::LoadTextureFromFile(L"./resources/BakerHouse/Baker_House.png");
	//App->GetTexture()->LoadTextureGPU();
	char* vSource = App->GetProgram()->LoadShaderSource("../Source/VertexShaderTexture.glsl");
	char* fSource = App->GetProgram()->LoadShaderSource("../Source/FragmentShaderTexture.glsl");
	GLuint vertexShader = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vSource);
	GLuint fragmentShader = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fSource);
	program = App->GetProgram()->CreateProgram(vertexShader, fragmentShader);

	float vtx_data[] = {
		0.5f, -0.5f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		-1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, //  v1 texcoord
		1.0f, 0.0f, //  v0 texcoord
		0.0f, 0.0f, // v2 texcoord
		0.0f, 1.0f,




		/*1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f*/
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	return true;
}

update_status ModuleBakerHouse::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleBakerHouse::Update()
{
	float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 3.0f),
		float4x4::RotateZ(0),
		float3(1.0f, 1.0f, 1.0f));
	//App->GetTexture()->LoadTextureGPU();
	App->GetProgram()->RenderVBO(vbo, program,texture, model);
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


