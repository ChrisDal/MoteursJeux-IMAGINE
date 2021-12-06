#include "VertexArrayBuffer.h"

VertexArrayBuffer::VertexArrayBuffer()
{
	glGenBuffers(1, &m_buffer); 
	glBindVertexArray(m_buffer); 
}

VertexArrayBuffer::~VertexArrayBuffer()
{
}

void VertexArrayBuffer::bind()
{
	glBindVertexArray(m_buffer); 
}

void VertexArrayBuffer::unbind()
{
	glBindVertexArray(0); 
}




