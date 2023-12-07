#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleOpenGL.h"
#include "SDL/include/SDL.h"
#include "imgui_impl_sdl2.h"
#include "ModuleWindow.h"
#include "ModelLoader.h"
#include "ModuleEditorCamera.h"
#include <iostream>
#include <string>


bool endsWith(const char* str, const char* suffix) {
    if (!str || !suffix)
        return false; //Change it for assert
    size_t strLength = std::strlen(str);
    size_t suffixLength = std::strlen(suffix);

    if (strLength < suffixLength)
        return false;

    return std::strcmp(str + std::strlen(str) - 5, suffix) == 0;

}

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;
    char* dropped_filedir;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;

            
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->GetOpenGL()->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;
            case SDL_DROPFILE:
                dropped_filedir = sdlEvent.drop.file;
                //TODO: Remove current Mesh object and load new file (check if file is .gltf or image in case loading texture.
                LOG(dropped_filedir);
                if (endsWith(dropped_filedir, ".gltf")) {
                    App->GetModelLoader()->loadModel(dropped_filedir);
                }
                else if (endsWith(dropped_filedir, ".png")) {
                    LOG("Check what is supposed to be the texture list in model loader, should I get the id of the textures and load the texture there?");
                }
                else {
                    LOG("File not valid");
                }
                SDL_free(dropped_filedir);
                
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
                    App->GetEditorCamera()->setCameraType(CameraType::movable);
                else if(canOrbit && sdlEvent.button.button == SDL_BUTTON_LEFT)
                    App->GetEditorCamera()->setCameraType(CameraType::orbit);
                break;

            case SDL_MOUSEBUTTONUP:
                if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
                    App->GetEditorCamera()->setCameraType(CameraType::fixed);
                if (sdlEvent.button.button == SDL_BUTTON_LEFT)
                    App->GetEditorCamera()->setCameraType(CameraType::fixed);
                break;
            
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

    if (keyboard[SDL_SCANCODE_ESCAPE])
    {
        return UPDATE_STOP;
    }
    if (keyboard[SDL_SCANCODE_LALT])
    {
        canOrbit = true;
    }

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
