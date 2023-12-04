#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleImGUI;
class ModuleRenderExercise;
class ModuleEditorCamera;
class TextureLoader;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput*  GetInput() { return input; }
    ModuleImGUI* GetImGUI() { return imgui; }
    ModuleEditorCamera* GetEditorCamera() { return camera; };
    ModuleRenderExercise* GetRenderExercise() { return r_ex; }
    TextureLoader* GetTextureLoader() { return text_loader; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleImGUI* imgui = nullptr;
    ModuleRenderExercise* r_ex = nullptr;
    ModuleEditorCamera* camera = nullptr;
    TextureLoader* text_loader = nullptr;


    std::list<Module*> modules;

};

extern Application* App;
