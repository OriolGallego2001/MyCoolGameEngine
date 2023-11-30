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
	void move(const float3& delta);
	void rotate(float angle, const float3& axis);
	void LookAt(const float3& to);

	const float4x4& GetViewMatrix() const { return ViewProjMatrix; }
	const float4x4& GetInvViewMatrix() const { return InvViewProjMatrix; }


private:
	Frustum frustum;
	float3 center; //Where the camera is looking at
	float4x4 ViewProjMatrix;
	float4x4 InvViewProjMatrix;


};

