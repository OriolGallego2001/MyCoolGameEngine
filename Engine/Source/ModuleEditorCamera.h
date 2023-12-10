#pragma once
#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"

enum CameraType
{
	fixed,
	movable,
	orbit
};

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
	void moveForward(bool backwards);
	void rotate(float angle, const float3& axis);

	void setCameraType(CameraType type) { cameraType = type; }

	const float4x4& GetViewProjMatrix() const { return frustum.ViewProjMatrix(); }
	const float4x4& GetInvViewProjMatrix() const { float4x4 vpMat = frustum.ViewProjMatrix(); vpMat.Inverse(); return vpMat; }
	const float3& GetCameraPos() const { return frustum.pos; }
	void ProcessInput();
	void UpdateProjectionMatrix(int screenWidth, int screenHeight);
	void DisplaceFromBoundingBox(float3 mins, float3 maxs);

private:
	Frustum frustum;
	float2 mousePos;
	CameraType cameraType = fixed;



};

