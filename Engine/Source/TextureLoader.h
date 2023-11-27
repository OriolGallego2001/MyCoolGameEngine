#pragma once
#include "../glew-2.1.0/include/GL/glew.h"
#include "DirectXTex.h"

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();


	bool LoadTexture(const char* filePath);
	void BindTexture();
	void UnbindTexture();

private:
	GLuint textureID;

};

