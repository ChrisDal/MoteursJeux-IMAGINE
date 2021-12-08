#pragma once
#include <iostream> 
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IndexBuffer
{
private:
	unsigned int m_buffer;
	GLenum m_usage; 
	unsigned int m_count; 

public:

	IndexBuffer(const unsigned int* data, unsigned int count, GLenum usage = GL_STATIC_DRAW);
	~IndexBuffer();

	void setData(const unsigned int* data, unsigned int count, GLenum usage = GL_STATIC_DRAW);
	GLenum getUsage() const; 

	void bind() const;
	void unbind() const;
	inline unsigned int getCount() const { return m_count; }

};