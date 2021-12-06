#include <iostream> 
#include "VertexArrayBuffer.h"

class VertexBuffer
{
private:
	unsigned int m_buffer;

public:
	VertexBuffer(VertexArrayBuffer* vao) {
		glGenBuffers(1, &m_buffer);
	}

	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	}


};