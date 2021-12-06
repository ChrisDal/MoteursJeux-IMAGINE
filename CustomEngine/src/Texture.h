#pragma once
#include <glad/glad.h>
#include "STB/stb_image.h"



class Texture
{
private: 
	unsigned int m_renderId; 
public: 
	Texture(); 
	Texture(const char* imagepath); 
	~Texture(); 

	void bind() const;
	void unbind() const; 

	// border - clamp 
	void setGenericParameters(); 

	unsigned int getRenderId() const; 
	void setData(unsigned char* data, int width, int height); 



};

