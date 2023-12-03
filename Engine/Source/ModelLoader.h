#pragma once

#include <vector>
#include "Mesh.h"


class ModelLoader
{
public:
    ModelLoader();
    ~ModelLoader();
    void loadModel(const char* path);

    // Add more functions as needed, e.g., for drawing the model

private:
    tinygltf::Model model;
    std::vector<float> positions;
    std::vector<float> texCoords;
    



};

