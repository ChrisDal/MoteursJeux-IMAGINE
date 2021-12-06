#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.h"

class VertexArrayBuffer
{
private:
	unsigned int m_buffer;

public: 

	VertexArrayBuffer(); 
	~VertexArrayBuffer(); 

	void bind(); 
	void unbind(); 

	void addBuffer(const VertexBuffer& vbuffer, const VertexBufferLayout& layout);
	void addBuffer(const VertexBuffer* vbuffer, const VertexBufferLayout& layout); 

};

