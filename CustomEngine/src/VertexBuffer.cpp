#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, GLenum usage)
	: m_buffer(0), m_usage(usage)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, m_usage);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_buffer); 
}

void VertexBuffer::setData(const void* data, unsigned int size, GLenum usage)
{
	m_usage = usage; 
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, m_usage);
}

GLenum VertexBuffer::getUsage() const
{
	return m_usage; 
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
}


void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
