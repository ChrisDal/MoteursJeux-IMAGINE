#include "Renderer.h"
#include "../GameObject.hpp"
#include "../SceneNode.h"
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
	: m_id(0), vpmat(glm::mat4(1.0f)), m_polymode(GL_FILL)
{
	//glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	// glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);
	setviewprojMat(glm::mat4(1.0f), glm::mat4(1.0f));
}

void Renderer::initGraphics() const
{
	// On init Black Frame 
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));

	// Enable depth buffer
	GLCall(glEnable(GL_DEPTH_TEST));

	// Enable back face culling
	//glEnable(GL_CULL_FACE);

	// GL_FRONT_AND_BACK for front- and back-facing polygons
	// GL_FILL : The interior of the polygon is filled
	GLCall(glPolygonMode(GL_FRONT_AND_BACK, m_polymode));
	
}

void Renderer::Clear() const
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GLCall(glPolygonMode(GL_FRONT_AND_BACK, m_polymode));


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

	GLCall(glDrawElements(mesh->getPrimitives(), mesh->getIndicesCount(), GL_UNSIGNED_INT, nullptr));
}


// IF no Shader attach to mesh use default shader 
void Renderer::Draw(GameObject* gmo, int shadertype) const
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
	glm::mat4 modelTransfointern = t_interne.getMatrixTransform() * meshobject->getModelView(); 
	chosenShader->setMat4("u_internal_tsfm_matrix", glm::value_ptr(modelTransfointern));
	
	// View Proj Matrix 
	chosenShader->setMat4("u_vp", glm::value_ptr(vpmat));
	unsigned int idcount = meshobject->getIndicesCount(); 
	// DRAW CALL 
	GLCall(glDrawElements(meshobject->getPrimitives(), idcount, GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(SceneNode* scene) const
{
	for (int k = 0; k < scene->getObjectNumber(); k++)
	{
		BasicGameObject* obj = scene->getObject(k); 
		if (obj->hasMesh())
		{
			this->Draw(static_cast<GameObject*>(obj), -1);
		}
		
	}

	for (int i = 0; i < scene->getChildrenNumber(); i++)
	{
		this->Draw(scene->getNode(i));
	}
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

void Renderer::setPolymode(bool polygon)
{
	m_polymode = polygon ? GL_FILL : GL_LINE; 
}


void Renderer::setviewprojMat(const glm::mat4& camView, const glm::mat4& camProj){
	

	// Set View-Projection Matrix 
	vpmat = camProj * camView;
	
}