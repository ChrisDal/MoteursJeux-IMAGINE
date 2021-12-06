#include "Texture.h"
#include <iostream>

Texture::Texture()
	: m_renderId(0)
{
	glGenTextures(1, &m_renderId); 
	// Set parameters 
	this->setGenericParameters();
	this->unbind(); 
}

Texture::Texture(const char* imagepath)
	: m_renderId(0)
{
	// Generate Texutre buffer 
	glGenTextures(1, &m_renderId);
	// Set parameters 
	this->setGenericParameters(); 

	// Load image 
	int width, height, nrChannels;
	unsigned char* data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
	
	if (data != nullptr)
	{
		// set Data to Texture 
		this->setData(data, width, height);
		// remove data loaded 
		stbi_image_free(data);
	}
	else
	{
		std::cout << "[STBI] Load Image Error : " << imagepath << std::endl;
	}
	
	
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_renderId); 
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_renderId);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setGenericParameters()
{
	this->bind(); 
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

unsigned int Texture::getRenderId() const
{
	return m_renderId;
}

void Texture::setData(unsigned char* data, int width, int height)
{
	this->bind(); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
	glGenerateMipmap(GL_TEXTURE_2D);
}
