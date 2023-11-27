#include "TextureLoader.h"


TextureLoader::TextureLoader() {
    glGenTextures(1, &textureID);
}

TextureLoader::~TextureLoader() {
    glDeleteTextures(1, &textureID);
}


bool TextureLoader::LoadTexture(const char* filePath) {

    wchar_t wideFilePath[MAX_PATH];
    mbstowcs(wideFilePath, filePath, MAX_PATH);

    // Use DirectXTex to load the texture
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage scratchImage;
    HRESULT result = DirectX::LoadFromDDSFile(
        wideFilePath, DirectX::DDS_FLAGS_NONE, &metadata, scratchImage);
    if (!SUCCEEDED(result)) {
        result = DirectX::LoadFromTGAFile(
            wideFilePath, DirectX::TGA_FLAGS_NONE, &metadata, scratchImage);
        if (!SUCCEEDED(result)) {
            result = DirectX::LoadFromWICFile(
                wideFilePath, DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
        }
    }
    
    if(SUCCEEDED(result)) {
        // Generate texture and set parameters
        // Generate texture and set parameters
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Specify texture parameters/options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Finally, load texture data to GPU
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            metadata.width,
            metadata.height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            scratchImage.GetPixels()
        );

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }
    else {
        return false;
    }
    
}

void TextureLoader::BindTexture() {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void TextureLoader::UnbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
