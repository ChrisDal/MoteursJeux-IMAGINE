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


Renderer::Renderer(float _w, float _h)
	: m_id(0), vpmat(glm::mat4(1.0f))
{
	//glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	// glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);
	setviewprojMat(_w, _h, glm::vec3(0.0, 0.0, -10.0f), true); 
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
	
	// View Proj Matrix 
	chosenShader->setMat4("u_vp", glm::value_ptr(vpmat));
	unsigned int idcount = meshobject->getIndicesCount(); 
	// DRAW CALL 
	GLCall(glDrawElements(mode, idcount, GL_UNSIGNED_INT, nullptr));
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
	Shader fragmentShader = Shader(fragmentshad.c_str(), GL_FRAGMENT_SHADER);

	vertexShader.checkValidity();
	fragmentShader.checkValidity();

	// create shaderProgram 
	m_shaderprograms.emplace_back(); 
	m_shaderprograms[m_shaderprograms.size()-1].bindShaders(&vertexShader, &fragmentShader);

	// link 
	m_shaderprograms[m_shaderprograms.size()-1].link();
} 

void Renderer::setviewprojMat(float width, float height, const glm::vec3& transview, bool orthographic){
	
	//-----------------
	// View Matrix 
	//-----------------

	//glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), transview);

	//-----------------
	// Projection Matrix 
	//-----------------
	glm::mat4 projection;
	float ratio = width / height; 
	if (orthographic) // TODO 
	{
		projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);
	}
	else 
	{
		projection = glm::perspective(glm::radians(35.0f), ratio, 0.1f, 100.0f);
	}


	// Set View-Projection Matrix 
	vpmat = projection * view; 
	
}