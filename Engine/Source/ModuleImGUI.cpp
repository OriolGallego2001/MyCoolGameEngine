#include "ModuleImGUI.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "../glew-2.1.0/include/GL/glew.h"
#include <cstring>
#include "ModuleEditorCamera.h"



ModuleImGUI::ModuleImGUI()
{

}

// Destructor
ModuleImGUI::~ModuleImGUI()
{
}

// Called before render is available
bool ModuleImGUI::Init()
{
	LOG("Creating ImGUI context");


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->getContext());
	ImGui_ImplOpenGL3_Init("#version 130");

	return true;
}

update_status ModuleImGUI::PreUpdate()
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleImGUI::Update()
{

	return UPDATE_CONTINUE;
}

update_status ModuleImGUI::PostUpdate()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImGui::ShowDemoWindow();
	//renderAboutWindow();
	renderLogWindow();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleImGUI::CleanUp()
{
	LOG("Destroying renderer");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	//Destroy window
	SDL_GL_DeleteContext(App->GetWindow()->window);

	return true;
}


void ModuleImGUI::renderAboutWindow() const
{
	// Begin the ImGui window
	ImGui::Begin("About...");

	// Display information
	ImGui::Text("Name of Your Engine: Uri's Super Cool Engine");
	ImGui::Text("One Line Description: This engine will be awesome!");
	ImGui::Text("Author: Oriol Gallego Vazquez");

	// Display library information
	ImGui::Text("Libraries Used:");
	ImGui::Text(" - Glew %s", glewGetString(GLEW_VERSION));  // Replace with the actual ImGui version
	ImGui::Text(" - ImGui %s", IMGUI_VERSION);  // Replace with the actual ImGui version
	ImGui::Text(" - OpenGL %s", glGetString(GL_VERSION));
	// Add more lines for other libraries you're using, similar to the above line

	// Display the license
	ImGui::Text("License: Your License");

	// End the ImGui window
	ImGui::End();
}

void ModuleImGUI::renderLogWindow() const
{
	ImGui::Begin("Console log:");

	float3 camerapos = App->GetEditorCamera()->GetCameraPos();
	ImGui::InputFloat3("CameraPos", &camerapos[0]);

	float3x3 cameraRotation = App->GetEditorCamera()->GetRotationMatrix();
	ImGui::InputFloat3("CameraRot1", &cameraRotation[0][0]);
	ImGui::InputFloat3("CameraRot2", &cameraRotation[1][0]);
	ImGui::InputFloat3("CameraRot3", &cameraRotation[2][0]);


	float4x4 viewproj = App->GetEditorCamera()->GetViewProjMatrix();
	ImGui::InputFloat4("CameraVP1", &viewproj[0][0]);
	ImGui::InputFloat4("CameraVP2", &viewproj[1][0]);
	ImGui::InputFloat4("CameraVP3", &viewproj[2][0]);
	ImGui::InputFloat4("CameraVP4", &viewproj[3][0]);

	// Display information
	/*
	for (const char* message : logMessages) {
		ImGui::TextUnformatted(message);
	}
	*/

	


	// End the ImGui window
	ImGui::End();
}


