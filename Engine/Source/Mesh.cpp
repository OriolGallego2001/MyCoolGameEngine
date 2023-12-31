#include "Mesh.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"
#include "SDL.h"
#include "../glew-2.1.0/include/GL/glew.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Application.h"

Mesh::Mesh() {

}

Mesh::~Mesh() {

}

void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	const auto& itPos = primitive.attributes.find("POSITION");
	const auto& itText = primitive.attributes.find("TEXCOORD_0");

	//TODO: Read UVs into vbo and use the space allocation properly for rendering.
	if (itPos != primitive.attributes.end() && itText != primitive.attributes.end())
	{
		LOG("Loading mesh %s ...", mesh.name);
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		const tinygltf::Accessor& textAcc = model.accessors[itText->second];
		SDL_assert(textAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(textAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& textView = model.bufferViews[textAcc.bufferView];
		const tinygltf::Buffer& textBuffer = model.buffers[textView.buffer];
		const unsigned char* bufferText = &(textBuffer.data[textAcc.byteOffset + textView.byteOffset]);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);
		float3* pos_ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		vertexCount = 0;
		size_t posByteStride = (posView.byteStride > 0) ? posView.byteStride : sizeof(float) * 3;
		size_t textByteStride = (posView.byteStride > 0) ? posView.byteStride : sizeof(float) * 2;


		for (size_t i = 0; i < posAcc.count; ++i)
		{
			pos_ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += posByteStride;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glGenBuffers(1, &uv_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float) * 2 * textAcc.count, nullptr, GL_STATIC_DRAW);
		float2* text_ptr = reinterpret_cast<float2*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		for (size_t i = 0; i < textAcc.count; ++i)
		{
			text_ptr[i] = *reinterpret_cast<const float2*>(bufferText);
			bufferText += textByteStride;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);


		materialIndex = primitive.material;
		vertexCount = posAcc.count;
		uvCount = textAcc.count;
		name = mesh.name.c_str();
		LOG("Mesh %s loaded!", mesh.name);

	}
	else if (itPos != primitive.attributes.end())
	{
		LOG("Loading mesh %s ...", mesh.name);

		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		vertexCount = 0;
		size_t byteStride = (posView.byteStride > 0) ? posView.byteStride : sizeof(float) * 3;

		for (size_t i = 0; i < posAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += byteStride;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		materialIndex = primitive.material;
		vertexCount = posAcc.count;
		name = mesh.name.c_str();
		texName = model.images[materialIndex].name.c_str();
		LOG("Mesh %s loaded!", mesh.name);

	}

}

void Mesh::Render(unsigned int program)
{
	
	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);

}

void Mesh::Draw(unsigned int program_id, const std::vector<unsigned>& textures)
{
	glUseProgram(program_id);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textures[materialIndex]);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
}


void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive) {
	if (primitive.indices >= 0)
	{
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset +
			indView.byteOffset]);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indAcc.count; ++i) ptr[i] = bufferInd[i];
		}
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			const uint8_t* bufferInd = reinterpret_cast<const uint8_t*>(buffer);
			for (uint8_t i = 0; i < indAcc.count; ++i) ptr[i] = bufferInd[i];
		}

		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const uint16_t* bufferInd = reinterpret_cast<const uint16_t*>(buffer);
			for (uint16_t i = 0; i < indAcc.count; ++i) ptr[i] = bufferInd[i];
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		atribCount = indAcc.count;
	}

}


