#include "ModuleEditorCamera.h"
#include "SDL.h"
#include "../glew-2.1.0/include/GL/glew.h"

ModuleEditorCamera::ModuleEditorCamera()
{
}

ModuleEditorCamera::~ModuleEditorCamera()
{
}

void ModuleEditorCamera::LookAt(const float3& to) {
    float4x4 m;
    float3 from = frustum.pos;
    float3 up = frustum.up;

    float3 forward = from - to;
    forward.Normalize();
    float3 right = up.Cross(forward);
    right.Normalize();
    float3 newup = forward.Cross(right);

    m[0][0] = right.x, m[0][1] = right.y, m[0][2] = right.z;
    m[1][0] = newup.x, m[1][1] = newup.y, m[1][2] = newup.z;
    m[2][0] = forward.x, m[2][1] = forward.y, m[2][2] = forward.z;
    m[3][0] = from.x, m[3][1] = from.y, m[3][2] = from.z;

    center = to;
    ViewProjMatrix = m * frustum.ProjectionMatrix();
    InvViewProjMatrix = ViewProjMatrix;
    InvViewProjMatrix.Inverse();
}


bool ModuleEditorCamera::Init()
{
    
    frustum.type = FrustumType::PerspectiveFrustum;
    frustum.pos = float3(0.0f, 1.0f, -2.0f);
    frustum.nearPlaneDistance = 0.1f;
    frustum.farPlaneDistance = 200.0f;
    frustum.horizontalFov = DegToRad(90.0f);
    frustum.verticalFov =  2.f * Atan(Tan(frustum.horizontalFov * 0.5f) / 1.3f);;
    frustum.up = float3::unitY;
    frustum.front = float3::unitZ;
    float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!
    LookAt(float3(1.0f, 1.0f, 1.0f));

    //Send the frustum projection matrix to OpenGL
    // direct mode would be:
    



    return true;
}

update_status ModuleEditorCamera::PreUpdate()
{
  
    return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::Update()
{
    //TODO: Create the input triggers
    return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::PostUpdate()
{
    return UPDATE_CONTINUE;
}

bool ModuleEditorCamera::CleanUp()
{
    return true;
}

void ModuleEditorCamera::move(const float3& delta)
{
    frustum.pos = frustum.pos - delta;
    frustum.pos = frustum.pos - delta;
    LookAt(center - delta);
    
}

void ModuleEditorCamera::rotate(float angle, const float3& axis)
{
    float4x4 R;
    R.RotateAxisAngle(axis, angle);
    float4 new_front4 = R * float4((center - frustum.pos),1);
    float3 new_front = new_front4.xyz() / new_front4.w;
    LookAt(frustum.pos + new_front);
}
