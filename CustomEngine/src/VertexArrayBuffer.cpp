#include "VertexArrayBuffer.h"
#include "Renderer.h"

VertexArrayBuffer::VertexArrayBuffer()
	: m_buffer(0)
{
	glGenVertexArrays(1, &m_buffer);
	glBindVertexArray(m_buffer); 
}

VertexArrayBuffer::~VertexArrayBuffer()
{
	glDeleteVertexArrays(1, &m_buffer);
}

void VertexArrayBuffer::bind()
{
	glBindVertexArray(m_buffer); 
}

void VertexArrayBuffer::unbind()
{
	glBindVertexArray(0); 
}


void VertexArrayBuffer::addBuffer(const VertexBuffer& vbuffer, const VertexBufferLayout& layout)
{
	this->bind();   // bind VAO 
	vbuffer.bind(); // bind Vertex Buffer
	const std::vector<VertexBufferElement> elements = layout.getElements(); 
	unsigned int offset = 0; 
	for (int i = 0; i < elements.size() ; i++)
	{
		const VertexBufferElement& element = elements[i]; 
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, 
									element.type, 
									element.normalized, 
									layout.getStride(), 
									(const void*)offset));

		offset += element.count * element.getSizeofType(element.type);
	}

}

void VertexArrayBuffer::addBuffer(const VertexBuffer* vbuffer, const VertexBufferLayout& layout)
{
	this->bind();   // bind VAO 
	vbuffer->bind(); // bind Vertex Buffer
	const std::vector<VertexBufferElement> elements = layout.getElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		const VertexBufferElement& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count,
			element.type,
			element.normalized,
			element.count * element.getSizeofType(element.type),
			(const void*)offset));

		offset += element.count * element.getSizeofType(element.type);
	}

}




