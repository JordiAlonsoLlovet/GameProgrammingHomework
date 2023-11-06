#include "Globals.h"
#include "ModuleCamera.h"
#include "MathGeoLib.h"

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