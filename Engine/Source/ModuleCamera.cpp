#include "Globals.h"
#include "ModuleCamera.h"
#include "MathGeoLib.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL.h"

ModuleCamera::ModuleCamera() {}
ModuleCamera::~ModuleCamera() {}

bool ModuleCamera::Init() {
	float aspect = SCREEN_HEIGHT / (double)SCREEN_WIDTH;
	camera.type = FrustumType::PerspectiveFrustum;
	camera.pos = float3(0.0f, 1.0f, 5.0f);
	camera.front = -float3::unitZ;
	camera.up = float3::unitY;
	camera.nearPlaneDistance = 0.1f;
	camera.farPlaneDistance = 100.0f;
	camera.horizontalFov = math::pi / 2.0f;
	camera.verticalFov = 2.f * atanf(tanf(camera.horizontalFov * 0.5f)*aspect);
    return true;
}

update_status ModuleCamera::Update() {
	float deltaMove = App->GetDeltaTime()* CAMERA_SPEED;
	float deltaTurn = App->GetDeltaTime() * CAMERA_TURNING_SPEED;
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
	return 1;
}

bool ModuleCamera::LookAt(float x, float y, float z)
{
	float3 target = float3(x, y, z);
	float3 direction = (target - camera.pos);
	direction.Normalize();
	camera.front = direction;
	return 1;
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