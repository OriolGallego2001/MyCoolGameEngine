#include "TextureLoader.h"


TextureLoader::TextureLoader() {
    glGenTextures(1, &textureID);
}

TextureLoader::~TextureLoader() {
    glDeleteTextures(1, &textureID);
}


bool TextureLoader::LoadTexture(const wchar_t* filePath) {


    // Use DirectXTex to load the texture
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage scratchImage;
    HRESULT result = DirectX::LoadFromDDSFile(
        filePath, DirectX::DDS_FLAGS_NONE, &metadata, scratchImage);
    if (!SUCCEEDED(result)) {
        result = DirectX::LoadFromTGAFile(
            filePath, DirectX::TGA_FLAGS_NONE, &metadata, scratchImage);
        if (!SUCCEEDED(result)) {
            result = DirectX::LoadFromWICFile(
                filePath, DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
        }
    }
    
    if(SUCCEEDED(result)) {
        // Generate texture and set parameters
        // Generate texture and set parameters
        int internalFormat, format, type;
        switch (metadata.format)
        {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_BGRA;
            type = GL_UNSIGNED_BYTE;
            break;
        case DXGI_FORMAT_B5G6R5_UNORM:
            internalFormat = GL_RGB8;
            format = GL_BGR;
            type = GL_UNSIGNED_BYTE;
            break;
        default:
            assert(false && "Unsupported format");
        }
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
            internalFormat,
            metadata.width,
            metadata.height,
            0,
            format,
            type,
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
