#include "Module.h"
#include "MathGeoLib.h"

class Application;

class ModuleCamera : public Module
{
public:

	ModuleCamera();
	virtual ~ModuleCamera();

	bool Init();
	update_status Update();
	bool CleanUp();
	float4x4 GetView();
	float4x4 GetProjection();
	bool SetFOV(float fov);
	bool SetAspectRatio(float aspect);
	bool LookAt(float x, float y, float z);
	float3 MoveCamera(float3 movement);
	void RotateH(double alpha);
	void RotateV(double alpha);
	void Rotate(double alpha, float3 axis);

private:
	Frustum camera;
	float currentAspect;

};
