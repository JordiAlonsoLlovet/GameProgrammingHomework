#include "Globals.h"
#include "ModuleRenderExercise.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "SDL.h"
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
	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, -2.0f),
		float4x4::RotateZ(pi / 4),
		float3(1.0f, 1.0f, 1.0f));

	texture = App->GetTexture()->LoadTextureFromFile(L"./resources/Test-image-Baboon.ppm");
	
	char* vSource = ModuleProgram::LoadShaderSource("../Source/VertexShaderTexture.glsl");
	char* fSource = ModuleProgram::LoadShaderSource("../Source/FragmentShaderTexture.glsl");
	GLuint vertexShader = ModuleProgram::CompileShader(GL_VERTEX_SHADER, vSource);
	GLuint fragmentShader = ModuleProgram::CompileShader(GL_FRAGMENT_SHADER, fSource);
	program = ModuleProgram::CreateProgram(vertexShader, fragmentShader);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]);

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

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	// 1. bind Vertex Array Object
	glBindVertexArray(vao);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof(float) * 4 * 3));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	return true;
}



// Called every draw update
update_status ModuleRenderExercice::Update()
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
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
	glUseProgram(0);
	
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


