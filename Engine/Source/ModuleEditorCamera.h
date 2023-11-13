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


private:
	Frustum frustum;
};

