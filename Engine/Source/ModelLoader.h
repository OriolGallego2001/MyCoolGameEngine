#pragma once

#include "../glew-2.1.0/include/GL/glew.h"
#include <vector>
#include <iostream>
#include "tiny_gltf.h"

using namespace tinygltf;

class ModelLoader
{
public:
    ModelLoader();
    ~ModelLoader();
    void loadModel(const char* path);

    // Add more functions as needed, e.g., for drawing the model

private:
    GLuint vbo;
    Model model;
    std::vector<float> positions;
    std::vector<float> texCoords;



};

