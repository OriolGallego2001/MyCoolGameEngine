#pragma once
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"
#include <vector>


class Mesh
{
public:
	Mesh();
	~Mesh();
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render(unsigned int program);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO();
	void Draw(unsigned int program_id, const std::vector<unsigned>& textures);
private:
	unsigned int vbo = 0, ebo = 0, vao = 0, uv_vbo = 0;
	int vertexCount = 0, atribCount = 0, uvCount;
	int materialIndex = -1;
};

