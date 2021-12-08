#pragma once
#define ASSERT(x) if (!(x)) __debugbreak(); 
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArrayBuffer.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "ShaderProgram.h"
#include "Mesh.h"



void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private: 
	unsigned int m_id; 
public: 

	Renderer(); 
	~Renderer() {};

	void Clear() const; 

	void Draw(const VertexArrayBuffer& vao, const IndexBuffer& ibo, const ShaderProgram& shader) const;
	void Draw(const VertexArrayBuffer* vao, const IndexBuffer* ibo, const ShaderProgram* shader) const;
	void Draw(Mesh* mesh, const ShaderProgram* shader) const;
	void Draw(Mesh* mesh, const std::string& shadername) const;
};

