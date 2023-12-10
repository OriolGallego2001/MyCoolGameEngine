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

#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <cstdlib>
#endif



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
	
	/*
	if (renderAbout)
	{
		renderAboutWindow();
	}
	renderLogWindow();
	renderProperties();
	renderProject();
	*/
	renderAboutWindow();

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

	if (forceClose) {
		return UPDATE_STOP;
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

	ImGui::Text("Performance:");

	float2 data = float2(80.0f, 60.0f);
	
	static float fpsValues[100] = { 0 }; // Circular buffer for the last 100 frames
	static int fpsIndex = 0;
	fpsValues[fpsIndex] = data.x;
	fpsIndex = (fpsIndex + 1) % 100;
	ImGui::PlotLines("FPS", fpsValues, 100, fpsIndex, "FPS", 0.0f, 100.0f, ImVec2(0, 100));

	// Plot Memory Usage (replace this with actual data retrieval)
	static float memoryValues[100] = { 0 }; // Circular buffer for the last 100 frames
	static int memoryIndex = 0;
	memoryValues[memoryIndex] = data.y;
	memoryIndex = (memoryIndex + 1) % 100;
	ImGui::PlotLines("Memory Usage", memoryValues, 100, memoryIndex, "MB", 0.0f, 100.0f, ImVec2(0, 100));


	// End the ImGui window
	ImGui::End();
}

void ModuleImGUI::renderLogWindow() const
{
	ImGui::Begin("Console log:");

	
	// Display information
	
	for (const char* message : logMessages) {
		ImGui::TextUnformatted(message);
	}
		


	// End the ImGui window
	ImGui::End();
}

void ModuleImGUI::renderProperties() const
{

	ImGui::Begin("Properties:");


	float3 camerapos = App->GetEditorCamera()->GetCameraPos();
	ImGui::InputFloat3("CameraPos", &camerapos[0]);



	float4x4 viewproj = App->GetEditorCamera()->GetViewProjMatrix();
	ImGui::InputFloat4("CameraVP1", &viewproj[0][0]);
	ImGui::InputFloat4("CameraVP2", &viewproj[1][0]);
	ImGui::InputFloat4("CameraVP3", &viewproj[2][0]);
	ImGui::InputFloat4("CameraVP4", &viewproj[3][0]);

	ImGui::End();


}

void ModuleImGUI::renderProject()
{
	ImGui::Begin("MySuperCoolEngine");

	if (ImGui::Button("Close Editor [Esc]"))
	{
		forceClose = true;
	}

	if (ImGui::Button("Open Repository"))
	{
		openRepositoryLink();
	}

	if (ImGui::Button("About window"))
	{
		renderAbout = !renderAbout;
	}

	ImGui::End();


}


void ModuleImGUI::openRepositoryLink()
{
#ifdef _WIN32
	// Convert wide string to narrow string
	const wchar_t* wideUrl = L"https://github.com/OriolGallego2001/MyCoolGameEngine";
	size_t convertedChars = 0;
	size_t size = wcslen(wideUrl) + 1;
	char narrowUrl[46]; // Size based on the length of the provided URL

	wcstombs_s(&convertedChars, narrowUrl, size, wideUrl, _TRUNCATE);

	// Use ShellExecuteA with the narrow string
	HINSTANCE result = ShellExecuteA(nullptr, nullptr, narrowUrl, nullptr, nullptr, SW_SHOWNORMAL);

	// Check the result if needed
	if ((intptr_t)result <= 32)
	{
		printf("Failed to open the link\n");
	}
#elif defined(__linux__)
	// Use xdg-open to open the default web browser on Linux
	system("xdg-open https://github.com/OriolGallego2001/MyCoolGameEngine");
#endif
}