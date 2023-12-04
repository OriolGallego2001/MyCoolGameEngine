#include "ModuleEditorCamera.h"
#include "SDL.h"
#include "../glew-2.1.0/include/GL/glew.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleEditorCamera::ModuleEditorCamera()
{
}

ModuleEditorCamera::~ModuleEditorCamera()
{
}




bool ModuleEditorCamera::Init()
{
    
    frustum.type = FrustumType::PerspectiveFrustum;
    frustum.pos = float3(0.0f, 1.0f, 2.0f);
    frustum.nearPlaneDistance = 0.1f;
    frustum.farPlaneDistance = 200.0f;
    frustum.horizontalFov = DegToRad(90.0f);
    frustum.verticalFov =  2.f * Atan(Tan(frustum.horizontalFov * 0.5f) / 1.3f);;
    
    frustum.front = float3(1,0,0);
    frustum.up = float3(0, 1, 0);
    int x, y;
    SDL_GetMouseState(&x, &y);
    mousePos = float2((float) x, (float) y);


    return true;
}

update_status ModuleEditorCamera::PreUpdate()
{
  
    return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::Update()
{
    //TODO: Create the input triggers
    ProcessInput();
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
    
    
}

void ModuleEditorCamera::rotate(float angle, const float3& axis)
{
    float3x3 rotationMatrix = float3x3::RotateAxisAngle(axis, angle);
    frustum.front = (rotationMatrix * frustum.front).Normalized();  
    frustum.up = (rotationMatrix * frustum.up).Normalized();    
    
    
}


void ModuleEditorCamera::ProcessInput()
{
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    float speed = 0.01f;

    int x, y;
    SDL_GetMouseState(&x, &y);
    float2 mouse_delta = float2(mousePos.x - x, mousePos.y - y);
    mousePos = float2((float)x, (float)y);
    
    if (keyboard[SDL_SCANCODE_LSHIFT])
    {
        speed = speed * 2;
    }
    if(cameraType == movable){
    
        if (keyboard[SDL_SCANCODE_W])
        {
            move(-frustum.front.Normalized() * speed);
        }

        if (keyboard[SDL_SCANCODE_S])
        {
            move(frustum.front.Normalized() * speed);

        }

        if (keyboard[SDL_SCANCODE_A])
        {
            move(frustum.WorldRight().Normalized() * speed);

        }

        if (keyboard[SDL_SCANCODE_D])
        {
            move(-frustum.WorldRight().Normalized() * speed);
        }

        if (keyboard[SDL_SCANCODE_Q])
        {
            move(frustum.up.Normalized() * speed);

        }
        if (keyboard[SDL_SCANCODE_E])
        {
            move(-frustum.up.Normalized() * speed);

        }

        if (mouse_delta.x != 0) {
            rotate(mouse_delta.x * 0.01, frustum.up);
        }
        if (mouse_delta.y != 0) {
            rotate(mouse_delta.y * 0.01, frustum.WorldRight());
        }

        App->GetWindow()->CenterHideMouse();

    }
    else if(cameraType == fixed) {

        if (keyboard[SDL_SCANCODE_UP])
        {
            rotate(speed, frustum.WorldRight());

        }
        if (keyboard[SDL_SCANCODE_DOWN])
        {
            rotate(-speed, frustum.WorldRight());

        }
        if (keyboard[SDL_SCANCODE_LEFT])
        {
            rotate(speed, frustum.up);

        }
        if (keyboard[SDL_SCANCODE_RIGHT])
        {
            rotate(-speed, frustum.up);

        }




    }
    






}
