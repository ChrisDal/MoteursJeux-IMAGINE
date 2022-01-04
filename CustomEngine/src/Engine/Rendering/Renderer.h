#pragma once
#define ASSERT(x) if (!(x)) __debugbreak(); 
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4

#include "VertexArrayBuffer.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.h"
#include "ShaderProgram.h"
#include "../Mesh.h"



class GameObject; 
class SceneNode; 

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private: 
	unsigned int m_id; 
	std::vector<ShaderProgram> m_shaderprograms; 
	
	// Set the projection view mat 
	glm::mat4 vpmat; 

	// Wireframe or polygon mode 
	GLenum m_polymode;

public: 

	Renderer(float _w, float _h); 
	
	~Renderer() {};

	void initGraphics() const; 
	void Clear() const; 

	void Draw(const VertexArrayBuffer& vao, const IndexBuffer& ibo, const ShaderProgram& shader) const;
	void Draw(const VertexArrayBuffer* vao, const IndexBuffer* ibo, const ShaderProgram* shader) const;
	void Draw(Mesh* mesh, const ShaderProgram* shader) const;
	void Draw(GameObject* gmo, int shadertype = -1) const;
	void Draw(SceneNode* scene) const;
	void Draw(Mesh* mesh, int shaderType) const;
	
	// Set View Proj Matrix
	void setviewprojMat(const glm::mat4& camView, const glm::mat4& camProj); 

	
	void createShaderProg(const std::string& vertexshad, const std::string& fragmentshad); 

	// Set Wireframe or polygon mode
	void setPolymode(bool polygon); 
	GLenum getPolymode() const { return m_polymode;  }

	// get Opengl Version 
	void printOpenGLVersion() const {
		const unsigned char* version = glGetString(GL_VERSION); 
		std::cout << "OpenGL Version : " << version << std::endl; 
	}
};

