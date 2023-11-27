#pragma once

#include "tiny_gltf.h"
#include "../glew-2.1.0/include/GL/glew.h"

class ModelLoader
{
public:
    ModelLoader();
    ~ModelLoader();

    bool LoadModel(const std::string& filePath);
    void CreateVBO();

    // Add more functions as needed, e.g., for drawing the model

private:
    GLuint vbo;
    tinygltf::Model model;
    std::vector<float> positions;
    std::vector<float> texCoords;

    // Helper function to extract vertex data from GLTF
    void ExtractVertexData();

};

