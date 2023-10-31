#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleImGUI : public Module
{
public:
	ModuleImGUI();
	~ModuleImGUI();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void renderAboutWindow();
	void renderLogWindow();


};

