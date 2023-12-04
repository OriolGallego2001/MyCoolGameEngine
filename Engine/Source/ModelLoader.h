#pragma once

#include <vector>
class Mesh;
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

class ModelLoader
{
public:
    ModelLoader();
    ~ModelLoader();
    void loadModel(const char* path);
    void loadlMaterials(const tinygltf::Model& srcModel);

    // Add more functions as needed, e.g., for drawing the model

private:
    std::vector<Mesh*> meshes;
    std::vector<unsigned int> textures;

};

