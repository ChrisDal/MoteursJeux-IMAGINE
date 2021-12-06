#pragma once
#include <glad/glad.h>

class VertexArrayBuffer
{
private: 
	unsigned int m_buffer; 

public: 

	VertexArrayBuffer(); 
	~VertexArrayBuffer(); 

	void bind(); 
	void unbind(); 

};

