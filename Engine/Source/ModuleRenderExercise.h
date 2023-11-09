#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"


class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	unsigned CompileShader(unsigned type, const char* source);
	char* LoadShaderSource(const char* shader_file_name);
	void RenderVBO(unsigned vbo, unsigned program);
	unsigned CreateTriangleVBO();
	void DestroyTriangleVBO();
	void RenderTriangle();
private:
	unsigned triangleVBO;
	unsigned fragment_id;
	unsigned vertex_id;
	unsigned program_id;
	Frustum frustum;
	float aspect;
	float4x4 proj, model, view;

	

};

