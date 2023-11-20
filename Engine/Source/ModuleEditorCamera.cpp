#include "ModuleEditorCamera.h"
#include "SDL.h"
#include "../glew-2.1.0/include/GL/glew.h"

ModuleEditorCamera::ModuleEditorCamera()
{
}

ModuleEditorCamera::~ModuleEditorCamera()
{
}

float4x4 LookAt(float3 from, float3 to, float3 up) {
    float4x4 m;
    float3 forward = from - to;
    forward.Normalize();
    float3 right = up.Cross(forward);
    right.Normalize();
    float3 newup = forward.Cross(right);

    m[0][0] = right.x, m[0][1] = right.y, m[0][2] = right.z;
    m[1][0] = newup.x, m[1][1] = newup.y, m[1][2] = newup.z;
    m[2][0] = forward.x, m[2][1] = forward.y, m[2][2] = forward.z;
    m[3][0] = from.x, m[3][1] = from.y, m[3][2] = from.z;

    return m;
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
    //Send the frustum projection matrix to OpenGL
    // direct mode would be:
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(*projectionGL.v);

    return true;
}

update_status ModuleEditorCamera::PreUpdate()
{
    float4x4 viewGL = float4x4(frustum.ViewMatrix()).Transposed();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(*viewGL.v);
    return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::Update()
{

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