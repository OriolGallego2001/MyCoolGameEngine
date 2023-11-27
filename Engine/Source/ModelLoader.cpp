#include "ModelLoader.h"


ModelLoader::ModelLoader() : vbo(0) {
    // Initialize any necessary OpenGL or other setup here
}

ModelLoader::~ModelLoader() {
    // Cleanup resources
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }
}

bool ModelLoader::LoadModel(const std::string& filePath) {
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filePath);

    if (!warn.empty()) {
        // Handle warnings if needed
    }

    if (!err.empty()) {
        // Handle errors
        return false;
    }

    return ret;
}

void ModelLoader::ExtractVertexData() {
    // Extract vertex data from the loaded GLTF model and store it in vectors
    // You'll need to adjust this based on your specific GLTF model structure

    // Example: Extracting positions and texture coordinates
    std::vector<GLfloat> positions;
    std::vector<GLfloat> texCoords;

    for (const auto& mesh : model.meshes) {
        for (const auto& primitive : mesh.primitives) {
            const auto& attributes = primitive.attributes;

            // Extract positions
            if (attributes.find("POSITION") != attributes.end()) {
                const auto& accessor = model.accessors[attributes.at("POSITION")];
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                const auto& buffer = model.buffers[bufferView.buffer];

                const float* positionsData = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                for (size_t i = 0; i < accessor.count * 3; ++i) {
                    positions.push_back(positionsData[i]);
                }
            }

            // Extract texture coordinates
            if (attributes.find("TEXCOORD_0") != attributes.end()) {
                const auto& accessor = model.accessors[attributes.at("TEXCOORD_0")];
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                const auto& buffer = model.buffers[bufferView.buffer];

                const float* texCoordsData = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                for (size_t i = 0; i < accessor.count * 2; ++i) {
                    texCoords.push_back(texCoordsData[i]);
                }
            }
        }
    }

    // Now, you can use the 'positions' and 'texCoords' vectors to create your VBO
    // and set up your shader attributes.
}

void ModelLoader::CreateVBO() {
    // Generate and bind the VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Extract vertex data from GLTF model
    ExtractVertexData();

    // Upload data to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (positions.size() + texCoords.size()), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * positions.size(), positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * positions.size(), sizeof(GLfloat) * texCoords.size(), texCoords.data());

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
