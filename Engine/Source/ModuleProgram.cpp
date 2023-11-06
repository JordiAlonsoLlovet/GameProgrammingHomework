
#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "MathGeoLib.h"
#include "debug_draw/ModuleDebugDraw.h"
#include "SDL.h"

ModuleProgram::ModuleProgram() {}
ModuleProgram::~ModuleProgram() {}

bool ModuleProgram::Init() {
	char* vSource = LoadShaderSource("../Source/VertexShader.glsl");
	char* fSource = LoadShaderSource("../Source/FragmentShader.glsl");
	GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vSource);
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fSource);
	program = CreateProgram(vertexShader, fragmentShader);
	
	return 1;
}

update_status ModuleProgram::PreUpdate() {
	//glUseProgram(program);
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::Update() {
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::PostUpdate() {
	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp() {
	glDeleteProgram(program);
	return 1;
}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
{
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, shader_file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(size + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	return data;
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
{
	unsigned shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);
	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
	}
	return shader_id;
}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
	unsigned program_id = glCreateProgram();
	glAttachShader(program_id, vtx_shader);
	glAttachShader(program_id, frg_shader);
	glLinkProgram(program_id);
	int res;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program_id, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}
	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}

// This function must be called each frame for drawing the triangle
void ModuleProgram::RenderVBO(unsigned vbo)
{
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0.0f, 1.0f, 5.0f);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f));
	float4x4 proj = frustum.ProjectionMatrix();
	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, -2.0f),
		float4x4::RotateZ(pi / 4.0f),
		float3(1.0f, 1.0f, 1.0f));
	float4x4 view = frustum.ViewMatrix();
	
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
	int w; int h;
	SDL_GetWindowSize(App->GetWindow()->window, &w, &h);
	App->GetDD()->Draw(view, proj, w, h);
}