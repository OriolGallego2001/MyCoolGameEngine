#include "ModelLoader.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"
using namespace tinygltf;
#include "Application.h"



ModelLoader::ModelLoader() : vbo(0) {
    // Initialize any necessary OpenGL or other setup here
}

ModelLoader::~ModelLoader() {
    // Cleanup resources
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }
}

void ModelLoader::loadModel(const char* path)
{
    TinyGLTF gltfContext;
    std::string error, warning;
    bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, path);
    if (!loadOk)
    {
        LOG("Error loading %s: %s", path, error.c_str());
    }
    for (const auto& srcMesh : model.meshes)
    {
        for (const auto& primitive : srcMesh.primitives)
        {
            
            
            
        }
    }

}

