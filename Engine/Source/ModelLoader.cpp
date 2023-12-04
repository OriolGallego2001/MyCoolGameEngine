#include "ModelLoader.h"

#include "Application.h"
#include "Mesh.h"
#include "TextureLoader.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"
#include <codecvt>


ModelLoader::ModelLoader()  {
}

ModelLoader::~ModelLoader() {

}

void ModelLoader::loadModel(const char* path)
{
    
    assert(path != nullptr);
    
    tinygltf::TinyGLTF gltfContext;
    tinygltf::Model model;
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
            mesh->LoadEBO(model, srcMesh, primitive);
            mesh->CreateVAO();
            meshes.push_back(mesh);
            
            
        }
    }

    loadMaterials(model);
    
    

}

void ModelLoader::loadMaterials(const tinygltf::Model& srcModel)
{
    TextureLoader* mytex = new TextureLoader();
    for (const auto& srcMaterial : srcModel.materials)
    {
        unsigned int textureId = 0;
        if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
        {
            const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
            const tinygltf::Image& image = srcModel.images[texture.source];
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            std::wstring wideString = converter.from_bytes("Data/BakerHouse/" + image.uri);
            const wchar_t* imageuri = wideString.c_str();
            textureId = (mytex->LoadTexture(imageuri));
        }
        textures.push_back(textureId);
    }

}

void ModelLoader::Render(unsigned int program_id)
{
    for (const auto& mesh : meshes) {
        mesh->Render(program_id);
        mesh->Draw(program_id, textures);
    }

}

