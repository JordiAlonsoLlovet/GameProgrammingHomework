#include <GL/glew.h>
#include "Globals.h"
#include "ModuleRenderExercise.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"
#include "SDL.h"
#include "debug_draw/debugdraw.h"


ModuleRenderExercice::ModuleRenderExercice()
{
	
}

// Destructor
ModuleRenderExercice::~ModuleRenderExercice()
{
}

// Called before render is available
bool ModuleRenderExercice::Init()
{
	App->GetTexture()->LoadTextureFromFile(L"./Test-image-Baboon.ppm");
	App->GetTexture()->LoadTextureGPU();
	char* vSource = App->GetProgram()->LoadShaderSource("../Source/VertexShader.glsl");
	char* fSource = App->GetProgram()->LoadShaderSource("../Source/FragmentShader.glsl");
	GLuint vertexShader = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vSource);
	GLuint fragmentShader = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fSource);
	program = App->GetProgram()->CreateProgram(vertexShader, fragmentShader);

	float vtx_data[] = { 
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,

		//0.0f, 1.0f, //  v0 texcoord
		//1.0f, 1.0f, //  v1 texcoord
		//0.5f, 0.0f // v2 texcoord


		/*1.0f, 1.0f, 0.0f, 
		-1.0f, 1.0f, 0.0f, 
		-1.0f, -1.0f, 0.0f*/
		 };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data2), vtx_data, GL_STATIC_DRAW);

	return true;
}

update_status ModuleRenderExercice::PreUpdate()
{
	dd::axisTriad(float4x4::identity, 0.1f, 1.0f);
	dd::xzSquareGrid(-10, 10, 0.0f, 1.0f, dd::colors::Gray);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercice::Update()
{
	App->GetTexture()->LoadTextureGPU();
	App->GetProgram()->RenderVBO(vbo, program);
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercice::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercice::CleanUp()
{
	glDeleteProgram(program);
	return true;
}


