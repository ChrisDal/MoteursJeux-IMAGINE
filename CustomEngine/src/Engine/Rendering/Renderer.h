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
#include "Shader.h"
#include "ShaderProgram.h"
#include "../Mesh.h"

class GameObject; 

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private: 
	unsigned int m_id; 
	std::vector<ShaderProgram> m_shaderprograms; 

public: 

	Renderer(); 
	~Renderer() {};

	void Clear() const; 

	void Draw(const VertexArrayBuffer& vao, const IndexBuffer& ibo, const ShaderProgram& shader) const;
	void Draw(const VertexArrayBuffer* vao, const IndexBuffer* ibo, const ShaderProgram* shader) const;
	void Draw(Mesh* mesh, const ShaderProgram* shader) const;
	void Draw(GameObject* gmo, GLenum mode = GL_TRIANGLES, int shadertype = -1) const;
	void Draw(Mesh* mesh, int shaderType) const;

	
	void createShaderProg(const std::string& vertexshad, const std::string& fragmentshad); 
};

