#pragma once
#include "tiny_gltf.h"

using namespace tinygltf;

class GMesh
{
public:
	GMesh();
	~GMesh();
	void Load(const Model& model, const Mesh& mesh, const Primitive& primitive);
	void Render();
	void LoadEBO(const Model& model, const Mesh& mesh, const Primitive& primitive);
private:
	unsigned vbo, ebo;
};

