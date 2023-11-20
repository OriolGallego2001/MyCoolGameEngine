#pragma once
#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
class ModuleEditorCamera :
    public Module
{
public:
	ModuleEditorCamera();
	~ModuleEditorCamera();
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void move(float3 delta);
	void rotate(float angle, const float3& axis);
	void LookAt(float3 to);

	Frustum frustum;
	float3 center; //Where the camera is looking at
	float4x4 ViewProjMatrix;
	float4x4 InvViewProjMatrix;


};

