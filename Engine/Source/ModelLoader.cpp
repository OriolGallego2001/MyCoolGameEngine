#include "ModelLoader.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"
using namespace tinygltf;


ModelLoader::ModelLoader() : vbo(0) {
    // Initialize any necessary OpenGL or other setup here
}

ModelLoader::~ModelLoader() {
    // Cleanup resources
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }
}

