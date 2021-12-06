#pragma once
#include <vector>
#include <glad/glad.h>
#include "Renderer.h"

struct VertexBufferElement
{
	//unsigned int index; 
	unsigned int type;
	unsigned int count; 
	unsigned char normalized;
	//unsigned int stride;
	//const void* data;

	static unsigned int getSizeofType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_BYTE:	return 1; 
		case GL_UNSIGNED_INT:	return 4; 
		}
		ASSERT(false); 
		return 0; 
	}
}; 

class VertexBufferLayout
{
private: 
	std::vector<VertexBufferElement> m_elements; 
	unsigned int m_stride; 

public: 
	VertexBufferLayout()
		: m_stride(0)
	{

	}

	~VertexBufferLayout() {}; 

	template<typename T>
	void Push(unsigned int count) {
		static_assert(false); 
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE}); 
		m_stride += sizeof(GLfloat); // size in bytes 
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += sizeof(GLuint); // size in bytes 
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += sizeof(GLbyte); // size in bytes 
	}

	inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }
	inline unsigned int getStride() const { return m_stride;  }

};

