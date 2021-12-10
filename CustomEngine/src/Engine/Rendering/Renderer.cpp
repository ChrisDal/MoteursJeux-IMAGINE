#include "Renderer.h"
#include "../GameObject.hpp"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
};

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL ERROR] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}

	return true;
};


Renderer::Renderer()
	: m_id(0)
{

}

void Renderer::Clear() const
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT););
}

void Renderer::Draw(const VertexArrayBuffer& vao, const IndexBuffer& ibo, const ShaderProgram& shader) const
{
	shader.use(); 
	vao.bind(); 
	ibo.bind(); 

	GLCall(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::Draw(const VertexArrayBuffer* vao, const IndexBuffer* ibo, const ShaderProgram* shader) const
{
	shader->use();
	vao->bind();
	ibo->bind();

	GLCall(glDrawElements(GL_TRIANGLES, ibo->getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(Mesh* mesh, const ShaderProgram* shader) const
{

	shader->use();
	mesh->bindBuffers(); 

	GLCall(glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_INT, nullptr));
}

// Default DRAW MODE = GL_TRIANGLES 
// IF no Shader attach to mesh use default shader 
void Renderer::Draw(GameObject* gmo, GLenum mode, int shadertype) const
{
	Mesh * meshobject = gmo->getMesh(); 
	const ShaderProgram* chosenShader = nullptr; 

	// -----------
	// Set SHADER 
	// -----------
	if (meshobject->isShaderSet())
	{
		bool isUsed = meshobject->useShader(); 
		if (!isUsed)
		{
			std::cout << "[RENDER] Problem with shader setting, gameObjectID=" << gmo->getId() << std::endl; 
			return; 
		}

		chosenShader = meshobject->getShader(); 
		
	}
	else
	{
		// use default shader or shadertype if given
		if (m_shaderprograms.size() > 0)
		{
			if (shadertype > -1 && shadertype < m_shaderprograms.size())
			{
				chosenShader = &m_shaderprograms[shadertype];
			}
			else
			{
				chosenShader = &m_shaderprograms[0];
			}

			chosenShader->use();
		}
		else
		{
			std::cout << "[RENDER] ERROR No default Shader set." << std::endl;
			return; 
		}

	}

	// ------------
	// Bind buffers
	// -------------
	meshobject->bindBuffers();

	// set uniform transform 
	// set transformation matrix for each game object
	SpaceEngine::Transform t = gmo->getTransformation();
	chosenShader->setMat4("u_transform_matrix", glm::value_ptr(t.getMatrixTransform()));

	// set world position matrix
	chosenShader->setMat4("u_world_matrix", glm::value_ptr(gmo->getWorldMat()));

	// set internal transform matrix
	SpaceEngine::Transform t_interne = gmo->getTransformation(true);
	chosenShader->setMat4("u_internal_tsfm_matrix", glm::value_ptr(t_interne.getMatrixTransform()));

	// DRAW CALL 
	GLCall(glDrawElements(GL_TRIANGLES, meshobject->getIndicesCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(Mesh* mesh, int shaderType) const
{
	// choose shader in shader list 
	const ShaderProgram* chosenShader; 
	if (shaderType > -1 && shaderType < m_shaderprograms.size()) 
	{
		chosenShader = &m_shaderprograms[shaderType];
	}
	else
	{
		chosenShader = &m_shaderprograms[0];
	}
	
	this->Draw(mesh, chosenShader); 
}

void Renderer::createShaderProg(const std::string& vertexshad, const std::string& fragmentshad)
{ 
	// vertex & fragment shader
	Shader vertexShader = Shader(vertexshad.c_str(), GL_VERTEX_SHADER); 
	Shader fragmentShader = Shader(vertexshad.c_str(), GL_FRAGMENT_SHADER);

	vertexShader.checkValidity();
	fragmentShader.checkValidity();

	// create shaderProgram 
	m_shaderprograms.push_back(ShaderProgram()); 
	m_shaderprograms[m_shaderprograms.size()-1].bindShaders(&vertexShader, &fragmentShader);

	// link 
	m_shaderprograms[m_shaderprograms.size()-1].link();
} 
