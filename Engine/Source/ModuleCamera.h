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

private:
	Frustum camera;
	float fov;
	float aspect;

};
