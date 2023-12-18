#include "Module.h"
#include "Math/float3x4.h"
#include "Math/float4x4.h"
#include "Geometry/Frustum.h"

class Application;

class ModuleCamera : public Module
{
public:

	ModuleCamera();
	virtual ~ModuleCamera();

	bool Init();
	update_status Update();
	bool CleanUp();
	float3x4* GetView();
	float4x4* GetProjection();
	void SetFOV(float fov);
	void SetAspectRatio(float aspect);
	void LookAt(float x, float y, float z);
	void LookAt(float3 target);
	void Orbit(float delta);
	float3* MoveCamera(const float3& movement);
	void RotateH(double alpha);
	void RotateV(double alpha);
	void Rotate(double alpha, const float3& axis);

private:
	Frustum camera;
	float3x4 view;
	float4x4 projection;
	float currentAspect;
	bool orbiting = false;
	float fov = 90;
	float cameraSpeed = CAMERA_SPEED;

};
