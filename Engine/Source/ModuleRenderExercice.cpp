#include <GL/glew.h>
#include "Globals.h"
#include "ModuleRenderExercise.h"
#include "Application.h"
#include "ModuleProgram.h"
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
	float vtx_data[] = { 
		-1.0f, -1.0f, 0.0f, 
		1.0f, -1.0f, 0.0f, 
		0.0f, 1.0f, 0.0f };

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
	App->GetProgram()->RenderVBO(vbo);
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercice::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercice::CleanUp()
{

	return true;
}


