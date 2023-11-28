#include "Globals.h"
#include "ModuleRenderExercise.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"
#include "SDL.h"
#include "debug_draw/debugdraw.h"
#include "MathGeoLib.h"
#include <GL/glew.h>


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
	texture = ModuleTexture::LoadTextureFromFile(L"./Test-image-Baboon.ppm");
	
	char* vSource = App->GetProgram()->LoadShaderSource("../Source/VertexShaderTexture.glsl");
	char* fSource = App->GetProgram()->LoadShaderSource("../Source/FragmentShaderTexture.glsl");
	GLuint vertexShader = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vSource);
	GLuint fragmentShader = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, fSource);
	program = App->GetProgram()->CreateProgram(vertexShader, fragmentShader);

	float vtx_data[] = { 
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
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

update_status ModuleRenderExercice::PreUpdate()
{
	dd::axisTriad(float4x4::identity, 0.1f, 1.0f);
	dd::xzSquareGrid(-10, 10, 0.0f, 1.0f, dd::colors::Gray);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercice::Update()
{
	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, -2.0f),
		float4x4::RotateZ(pi/4),
		float3(1.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	App->GetProgram()->RenderVBO(vbo, program, texture, model);
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
	glDeleteTextures(1, &texture);
	return true;
}


