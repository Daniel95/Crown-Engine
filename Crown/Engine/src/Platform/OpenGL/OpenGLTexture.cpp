#include "CrownPCH.h"
#include "OpenGLTexture.h"
#include "stb_image.h"
#include "tiny_gltf.h"

void OpenGLTexture::Init()
{

	// maybe set values relating to transparancy etc. here?
	
}

std::shared_ptr<Crown::Texture> Crown::Texture::Create()
{
	return std::make_shared<OpenGLTexture>();
}

//Loads data through stb_image and creates the OpenGL handle
void OpenGLTexture::Load(const tinygltf::Image& a_MaterialData)
{
	LOG("Loading a texture");
	glGenTextures(1, &m_Handle);
	Bind();
	//glBindTexture(GL_TEXTURE_2D, m_Handle);

	//wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::string path = "assets/models/";
	path += a_MaterialData.uri;
	
	stbi_set_flip_vertically_on_load(true);
	m_Data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
	if(m_Data)
	{
		LOG("Loading: " << path.c_str());
		switch(m_Channels)
		{
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_MaterialData.width, a_MaterialData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_MaterialData.width, a_MaterialData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data);
			break;
		default:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, a_MaterialData.width, a_MaterialData.height, 0, GL_RED, GL_UNSIGNED_BYTE, m_Data);
			break;
		}
		
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG("Failed to load " << path.c_str());

		path = "assets/models/white.png";
		m_Data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
		if (m_Data)
		{
			LOG("Loading white");
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	Unbind();

	stbi_image_free(m_Data);
}

void OpenGLTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_Handle);
}

void OpenGLTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
