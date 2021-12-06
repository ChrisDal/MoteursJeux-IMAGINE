#include "IndexBuffer.hpp"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, GLenum usage)
	: m_buffer(0), m_usage(usage), m_count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint)); 
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), data, m_usage);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_buffer); 
}

void IndexBuffer::setData(const unsigned int* data, unsigned int count, GLenum usage)
{
	m_usage = usage; 
	m_count = count; 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), data, m_usage);
}

GLenum IndexBuffer::getUsage() const
{
	return m_usage; 
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
}


void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
