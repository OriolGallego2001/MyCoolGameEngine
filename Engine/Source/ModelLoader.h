#pragma once

#include <vector>
class Mesh;
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"
#include "Module.h"

class ModelLoader : public Module
{
public:
    ModelLoader();
    ~ModelLoader();
    bool Init();
    bool CleanUp();

    void loadModel(const char* path);
    void loadMaterials(const tinygltf::Model& srcModel);
    void Render(unsigned int program_id);

    const std::vector<Mesh*> getMeshes() const { return meshes; }

    // Add more functions as needed, e.g., for drawing the model

private:
    std::vector<Mesh*> meshes;
    std::vector<unsigned int> textures;

};

