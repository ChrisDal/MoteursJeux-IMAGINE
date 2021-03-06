#pragma once
#include <glad/glad.h>

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.h"

class VertexArrayBuffer
{
private:
	unsigned int m_buffer;

public: 

	VertexArrayBuffer(); 
	~VertexArrayBuffer(); 

	void bind() const; 
	void unbind() const;

	void addBuffer(const VertexBuffer& vbuffer, const VertexBufferLayout& layout);
	void addBuffer(const VertexBuffer* vbuffer, const VertexBufferLayout* layout); 

};

