#pragma once
#include "tiny_gltf.h"


class Mesh
{
public:
	Mesh();
	~Mesh();
	void Load(const tinygltf::Model& model, const Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render();
	void LoadEBO(const tinygltf::Model& model, const Mesh& mesh, const tinygltf::Primitive& primitive);
private:
	unsigned vbo, ebo;
};

