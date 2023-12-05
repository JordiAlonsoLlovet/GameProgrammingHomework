#include "Globals.h"
#include "ModuleCamera.h"
#include "MathGeoLib.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleTimer.h"
#include "SDL.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

ModuleCamera::ModuleCamera() {}
ModuleCamera::~ModuleCamera() {}

bool ModuleCamera::Init() {
	currentAspect = SCREEN_HEIGHT / (double)SCREEN_WIDTH;
	camera.type = FrustumType::PerspectiveFrustum;
	camera.pos = float3(0.0f, 1.0f, 5.0f);
	camera.front = -float3::unitZ;
	camera.up = float3::unitY;
	camera.nearPlaneDistance = 0.1f;
	camera.farPlaneDistance = 100.0f;
	camera.horizontalFov = math::pi / 2.0f;
	camera.verticalFov = 2.f * atanf(tanf(camera.horizontalFov * 0.5f)* currentAspect);
    return true;
}

update_status ModuleCamera::Update() {
	static float cameraSpeed = CAMERA_SPEED;
	static float f = 90;
	ImGui::SliderFloat("Movement speed", &cameraSpeed, 0.0f, CAMERA_SPEED*10);
	if (ImGui::SliderFloat("FOV", &f, 0, 180))
		SetFOV(pi * f / 180);
	float deltaMove = App->GetClock()->GetDeltaTime() * cameraSpeed / (double)CLOCKS_PER_SEC;
	float deltaTurn = App->GetClock()->GetDeltaTime() * CAMERA_TURNING_SPEED / (double)CLOCKS_PER_SEC;
	if (App->GetInput()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		deltaMove *= 2;
	float3 m = float3(0.0f, 0.0f, 0.0f);
	if (App->GetInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		m += camera.front.Normalized() * deltaMove;
	if (App->GetInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		m += camera.front.Normalized() * -deltaMove;
	if (App->GetInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		m += camera.front.Cross(camera.up).Normalized() * -deltaMove;
	if (App->GetInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		m += camera.front.Cross(camera.up).Normalized() * deltaMove;
	if (App->GetInput()->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		m.y += deltaMove;
	if (App->GetInput()->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		m.y -= deltaMove;

	if (App->GetInput()->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		RotateH(-deltaTurn);
	if (App->GetInput()->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		RotateH(deltaTurn);
	if (App->GetInput()->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		RotateV(deltaTurn);
	if (App->GetInput()->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		RotateV(-deltaTurn);
	
	camera.pos += m;
    return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
    return true;
}

float4x4 ModuleCamera::GetView()
{
	return camera.ViewMatrix();
}

float4x4 ModuleCamera::GetProjection()
{
	return camera.ProjectionMatrix();
}

bool ModuleCamera::SetAspectRatio(float aspect) {
	camera.verticalFov = 2.f * atanf(tanf(camera.horizontalFov * 0.5f) * aspect);
	currentAspect = aspect;
	return 1;
}

bool ModuleCamera::SetFOV(float fov) 
{
	camera.horizontalFov = fov;
	SetAspectRatio(currentAspect);
	return 1;
}

void ModuleCamera::LookAt(float x, float y, float z)
{
	float3 target = float3(x, y, z);
	LookAt(target);
}

float3 ModuleCamera::MoveCamera(float3 movement)
{
	camera.pos += movement;
	return camera.pos;
}

void ModuleCamera::RotateV(double alpha)
{
	float3 axis = camera.front.Cross(camera.up).Normalized();
	Rotate(alpha, axis);
}

void ModuleCamera::RotateH(double alpha) 
{
	float3 axis = float3::unitY; //Rotates around world axis
	//float3 axis = camera.up; //Rotates around local axis
	Rotate(alpha, axis);
}

void ModuleCamera::Rotate(double alpha, float3 axis)
{
	float3x3 cpMatrix = {
		0, -axis.z, axis.y,
		axis.z, 0, -axis.x,
		-axis.y, axis.x, 0 };
	float3x3 r = float3x3::identity * cos(alpha) + cpMatrix * sin(alpha) + axis.OuterProduct(axis) * (1 - cos(alpha));
	camera.front = r * camera.front;
	camera.up = r * camera.up;
}

void ModuleCamera::LookAt(float3 target)
{
	camera.pos = target - 3 * float3::unitZ;

	camera.front = float3::unitZ;
	camera.up = float3::unitY;
}