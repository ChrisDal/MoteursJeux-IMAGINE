#pragma once
#include <glad/glad.h>
#include "STB/stb_image.h"



class Texture
{
private: 
	unsigned int m_renderId; 
public: 
	Texture(); 
	Texture(const char* imagepath, unsigned int alphatype = 0);
	~Texture(); 

	// bind 
	void bind() const;
	void bind(unsigned char unit) const; 
	void bind(unsigned int unit) const;
	void bind(int unit) const;
	void unbind() const; 

	// activate unit texture 
	void activateTextureUnit(unsigned char unit = 0) const; 

	// border - clamp 
	void setGenericParameters(); 

	// data
	unsigned int getRenderId() const; 
	void setDataRGBAlpha(unsigned char* data, int width, int height);
	void setDataRGB(unsigned char* data, int width, int height);
	void setDataAlpha(unsigned char* data, int width, int height);




};

