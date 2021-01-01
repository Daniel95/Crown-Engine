#pragma once
#include "Core/Renderer/Texture.h"

class OpenGLTexture final : public Crown::Texture
{
public:
	void Init() override;
	void Load(const tinygltf::Image& a_MaterialData) override;
	void Bind() override;
	void Unbind() override;
	
private:
	unsigned char* m_Data = nullptr;
	unsigned int m_Handle = 0;
	int m_Width = 0, m_Height = 0, m_Channels = 0;
	glm::vec4 color; 
};

