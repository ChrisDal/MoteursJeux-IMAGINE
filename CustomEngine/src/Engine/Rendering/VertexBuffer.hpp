#pragma once 
#include <iostream> 
#include <glad/glad.h>

class VertexBuffer
{

private:
	unsigned int m_buffer;
	GLenum m_usage; 

public:

	VertexBuffer(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);
	~VertexBuffer();

	void setData(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);
	GLenum getUsage() const; 

	void bind() const;
	void unbind() const;

};