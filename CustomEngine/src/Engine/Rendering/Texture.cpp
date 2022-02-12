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

// alphatype = 0 : no alpha 
// alphatype = 1 : Alpha channel but use RGB 
// alphatype = 2 : Alpha channel and use RGBA 
Texture::Texture(const char* imagepath, unsigned int alphatype)
	: m_renderId(0)
{
	// Generate Texture buffer 
	glGenTextures(1, &m_renderId);
	// Set parameters 
	this->setGenericParameters(); 

	// Load image 
	int width, height, nrChannels;
	unsigned char* data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
	
	if (data != nullptr)
	{
		// set Data to Texture 
		if (alphatype == 0) {
			this->setDataRGB(data, width, height);
		}
		else if (alphatype == 1) {
			this->setDataRGBAlpha(data, width, height);
		}
		else if (alphatype == 2) {
			this->setDataAlpha(data, width, height);
		}
		else
		{
			this->setDataRGB(data, width, height);
		}
		
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
	std::cout << "[Texture] Destructor " << &m_renderId << std::endl; 
	glDeleteTextures(1, &m_renderId); 
}



void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_renderId);
}

void Texture::bind(unsigned char unit) const
{
	this->activateTextureUnit(unit); 
	this->bind();
}

void Texture::bind(unsigned int unit) const
{
	this->activateTextureUnit((unsigned char)unit);
	this->bind(); 
}

void Texture::bind(int unit) const
{
	this->activateTextureUnit((unsigned char)unit);
	this->bind();
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::activateTextureUnit(unsigned char unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
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

void Texture::setDataRGB(unsigned char* data, int width, int height)
{
	this->bind(); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::setDataAlpha(unsigned char* data, int width, int height)
{
	this->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}


void Texture::setDataRGBAlpha(unsigned char* data, int width, int height)
{
	this->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}
