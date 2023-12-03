#include "ModelLoader.h"

#include "Application.h"
#include "Mesh.h"


ModelLoader::ModelLoader()  {
}

ModelLoader::~ModelLoader() {

}

void ModelLoader::loadModel(const char* path)
{
    
    assert(path != nullptr);
    
    tinygltf::TinyGLTF gltfContext;
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
            Mesh* mesh = new Mesh();
            mesh->Load(model, srcMesh, primitive);
            
            
        }
    }
    
    

}

