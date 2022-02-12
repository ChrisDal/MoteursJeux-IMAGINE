#include "Renderer.h"
#include "../GameObject.hpp"
#include "../Light/LightObject.h"
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

Renderer::~Renderer()
{
	if (m_shaderprog_classic != nullptr) {
		delete m_shaderprog_classic; 
	}
	if (m_shaderprog_phong != nullptr) {
		delete m_shaderprog_phong;
	}
	if (m_shaderprog_other != nullptr) {
		delete m_shaderprog_other;
	}

	if (m_shaderprog_texture != nullptr) {
		delete m_shaderprog_texture;
	}
}

void Renderer::initGraphics() const
{
	// On init Black Frame 
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	
	// Enable back face culling
	//GLCall(glEnable(GL_CULL_FACE));

	// Enable depth buffer
	GLCall(glEnable(GL_DEPTH_TEST));

	// GL_FRONT_AND_BACK for front- and back-facing polygons
	// GL_FILL : The interior of the polygon is filled
	GLCall(glPolygonMode(GL_FRONT_AND_BACK, m_polymode));
	
	

}

void Renderer::Clear() const
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	// GL_FRONT_AND_BACK for front- and back-facing polygons
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
void Renderer::Draw(GameObject* gmo, const glm::vec4& cameraPos, LightObject* lgtobj, Material* mat, int shadertype) const
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
		//const int maxShader = 3; 

		Material* material = meshobject->getMat();
		// Force Shader Type if texture 
		if (material != nullptr && material->hasTexturing()) {
			shadertype = 3; 
		}
		

		switch (shadertype)
		{
		case(0): chosenShader = m_shaderprog_classic; break; 
		case(1): chosenShader = m_shaderprog_phong;   break; 
		case(2): chosenShader = m_shaderprog_other;   break;
		case(3): chosenShader = m_shaderprog_texture; break;
		default: 
			if (m_shaderprog_classic != nullptr)
			{
				chosenShader = m_shaderprog_classic;
			}
			else
			{
				std::cout << "[RENDER] ERROR No default Classic Shader set." << std::endl;
				return;
			}
			break;
		}


	}

	chosenShader->use();

	// ------------
	// Bind buffers
	// -------------
	meshobject->bindBuffers();

	// ------------
	// Set Uniforms 
	// -------------

	// set internal transform matrix
	SpaceEngine::Transform t_interne = gmo->getTransformation(); 
	glm::mat4 modelTransfointern = gmo->getNode()->getMatTotalNodeTransform() * meshobject->getModelView();
	chosenShader->setMat4("u_transform", glm::value_ptr(modelTransfointern));

	// set matrix transform for normals 
	glm::mat4 normTransform = SpaceEngine::getMatNormal(modelTransfointern); 
	chosenShader->setMat4("u_Ntransform", glm::value_ptr(normTransform)); 
	
	// View Proj Matrix 
	chosenShader->setMat4("u_viewproj", glm::value_ptr(vpmat));
	

	glm::vec4 color = meshobject->getColor(); 
	chosenShader->setUniform4f("u_color", color.x, color.y, color.z, color.w);

	// Material // Texture 
	bool texturebind = false;
	Material* material = meshobject->getMat();
	
	if (material != nullptr)
	{
		// diffuse spec 
		if (material->hasTexturing())
		{
			// Texture Unit  0 and 1 
			unsigned int textUnitStart = 0; 
			chosenShader->setUniform1i("u_material.diffuse", textUnitStart);
			chosenShader->setUniform1i("u_material.specular", textUnitStart + 1);
			chosenShader->setUniform1f("u_material.shininess", material->m_shininess);
			material->bindTextures(textUnitStart);
			texturebind = true;
		}
		// simple texture 
		// TODO : remove 
		else if (material->hasTexture())
		{
			material->getTexture()->bind(0);
			
			texturebind = true;
		}
		else
		{
			chosenShader->setUniform3f("u_material.ambient", material->m_ambient);
			chosenShader->setUniform3f("u_material.diffuse", material->m_diffuse);
			chosenShader->setUniform3f("u_material.specular", material->m_specular);
			chosenShader->setUniform1f("u_material.shininess", material->m_shininess);
		}
	}

	

	// Light Color 
	if (lgtobj != nullptr) {
		chosenShader->setUniform4f("u_light.color", lgtobj->getColor());
		chosenShader->setUniform4f("u_light.position", lgtobj->getWorldPosition());

		//  Light Material 
		Material* lightmat = lgtobj->getMaterial(); 
		chosenShader->setUniform4f("u_light.ambient", glm::vec4(lightmat->m_ambient, 1.0f));
		chosenShader->setUniform4f("u_light.diffuse", glm::vec4(lightmat->m_diffuse, 1.0f));
		chosenShader->setUniform4f("u_light.specular",glm::vec4(lightmat->m_specular, 1.0f));
	} 
	else
	{
		chosenShader->setUniform4f("u_lightColor", glm::vec4(1.0f)); 
		chosenShader->setUniform4f("u_lightPos", 0.0f, 0.0f, 0.0f, 1.0f);
	}

	// Camera Position for the view 
	chosenShader->setUniform4f("u_viewPos", cameraPos); 
	
	

	

	// DRAW CALL 
	unsigned int idcount = meshobject->getIndicesCount();
	GLCall(glDrawElements(meshobject->getPrimitives(), idcount, GL_UNSIGNED_INT, nullptr));

	if (texturebind) { 
		if (material->hasTexturing()) {
			material->unbindTextures(); 

		}
		else if (material->hasTexture()) {
			material->getTexture()->unbind(); 
		}
	}
	chosenShader->unuse();
}

// IF no Shader attach to mesh use default shader 
void Renderer::Draw(LightObject* lgtobj, Material* mat, int shadertype) const
{

	Mesh* meshobject = lgtobj->getMesh();
	const ShaderProgram* chosenShader = nullptr;

	// -----------
	// Set SHADER 
	// -----------
	if (meshobject->isShaderSet())
	{
		bool isUsed = meshobject->useShader();
		if (!isUsed)
		{
			std::cout << "[RENDER] Problem with shader setting, lightObjectID=" << lgtobj->getId() << std::endl;
			return;
		}

		chosenShader = meshobject->getShader();

	}
	else
	{
		// use default shader or shadertype if given
		if (m_shaderprog_phong != nullptr)
		{
			chosenShader = m_shaderprog_phong; 
		}
		else
		{
			std::cout << "[RENDER] ERROR No default Shader Phong set." << std::endl;
			return;
		}

	}

	chosenShader->use();

	// ------------
	// Bind buffers
	// -------------
	meshobject->bindBuffers();

	// ------------
	// Set Uniforms 
	// -------------

	// set internal transform matrix
	SpaceEngine::Transform t_interne = lgtobj->getTransformation();
	glm::mat4 modelTransfointern = lgtobj->getNode()->getMatTotalNodeTransform()   * meshobject->getModelView();
	chosenShader->setMat4("u_transform", glm::value_ptr(modelTransfointern));

	// View Proj Matrix 
	chosenShader->setMat4("u_viewproj", glm::value_ptr(vpmat));
	unsigned int idcount = meshobject->getIndicesCount();

	glm::vec4 color = meshobject->getColor();
	chosenShader->setUniform4f("u_color", color.x, color.y, color.z, color.w);

	// Texture 
	bool texturebind = false;
	if (lgtobj->getMaterial() != nullptr)
	{
		//  Light Material 
		Material* lightmat = lgtobj->getMaterial();
		/*chosenShader->setUniform4f("u_light.ambient", glm::vec4(lightmat->m_ambient, 1.0f));
		chosenShader->setUniform4f("u_light.diffuse", glm::vec4(lightmat->m_diffuse, 1.0f));
		chosenShader->setUniform4f("u_light.specular", glm::vec4(lightmat->m_specular, 1.0f));*/
		
		
		if (lgtobj->getMaterial()->getTexture() != nullptr)
		{
			lgtobj->getMaterial()->getTexture()->bind(0);
			texturebind = true;
		}
	}

	// DRAW CALL 
	GLCall(glDrawElements(meshobject->getPrimitives(), idcount, GL_UNSIGNED_INT, nullptr));

	if (texturebind) { mat->getTexture()->unbind(); }
	chosenShader->unuse(); 
}

// Draw a scene 
void Renderer::Draw(SceneNode* scene, const glm::vec4& cameraPos) const
{
	std::vector<LightObject*> lights = scene->getLights(); 
	
	
	if (scene->haveGmo()) {
		BasicGameObject* obj = scene->getObject();
		if (obj->hasMesh())
		{
			if ( ! obj->isLight())
			{
				this->Draw(static_cast<GameObject*>(obj), cameraPos, lights[0],  nullptr, RENDERING_STYLE::CLASSIC);
			}
			else
			{
				this->Draw(static_cast<LightObject*>(obj), nullptr, RENDERING_STYLE::CLASSIC);
			}
		}
	}


	for (int i = 0; i < scene->getChildrenNumber(); i++)
	{
		this->Draw(scene->getNode(i), cameraPos, lights[0]);
	}
}

void Renderer::Draw(SceneNode* scene, const glm::vec4& cameraPos, LightObject* light) const
{
	if (scene->haveGmo()) {
		BasicGameObject* obj = scene->getObject();
		if (obj->hasMesh())
		{
			if (! obj->isLight())
			{
				this->Draw(static_cast<GameObject*>(obj), cameraPos, light, nullptr, RENDERING_STYLE::CLASSIC);
			}
			else
			{
				this->Draw(static_cast<LightObject*>(obj), nullptr, RENDERING_STYLE::CLASSIC);
			}
		}
	}


	for (int i = 0; i < scene->getChildrenNumber(); i++)
	{
		this->Draw(scene->getNode(i), cameraPos, light);
	}
}

void Renderer::Draw(SceneNode* scene, const glm::vec4& cameraPos, std::vector<LightObject*> lights) const
{
	for (LightObject* light : lights)
	{

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

void Renderer::createShaderProg(const std::string& vertexshad, const std::string& fragmentshad, Renderer::RENDERING_STYLE style)
{
	// vertex & fragment shader
	Shader vertexShader = Shader(vertexshad.c_str(), GL_VERTEX_SHADER);
	Shader fragmentShader = Shader(fragmentshad.c_str(), GL_FRAGMENT_SHADER);

	bool isvalid = vertexShader.checkValidity();
	isvalid &= fragmentShader.checkValidity();

	if (!isvalid) { return;  }
	
	switch (style)
	{
	case(0) : 
		if (m_shaderprog_classic != nullptr) {
			delete m_shaderprog_classic;
		}
		
		m_shaderprog_classic = new ShaderProgram(); 
		m_shaderprog_classic->bindShaders(&vertexShader, &fragmentShader);
		m_shaderprog_classic->link();
		break;
	case(1) :
		if (m_shaderprog_phong != nullptr) {
			delete m_shaderprog_phong;
		}
		
		m_shaderprog_phong = new ShaderProgram();
		m_shaderprog_phong->bindShaders(&vertexShader, &fragmentShader);
		m_shaderprog_phong->link();
		break;
	case(2): 
		if (m_shaderprog_other != nullptr) {
			delete m_shaderprog_other;
		}
		m_shaderprog_other = new ShaderProgram();
		m_shaderprog_other->bindShaders(&vertexShader, &fragmentShader);
		m_shaderprog_other->link();
		break; 
	case(3): 
		if (m_shaderprog_texture != nullptr) {
			delete m_shaderprog_texture;
		}
		m_shaderprog_texture = new ShaderProgram();
		m_shaderprog_texture->bindShaders(&vertexShader, &fragmentShader);
		m_shaderprog_texture->link();
		break;
	default: 
		if (m_shaderprog_classic != nullptr) {
			delete m_shaderprog_classic;
		}
		m_shaderprog_classic = new ShaderProgram();
		m_shaderprog_classic->bindShaders(&vertexShader, &fragmentShader);
		m_shaderprog_classic->link();
		break;
	}
}

void Renderer::setPolymode(bool polygon)
{
	m_polymode = polygon ? GL_FILL : GL_LINE; 
}


void Renderer::setviewprojMat(const glm::mat4& camView, const glm::mat4& camProj){
	

	// Set View-Projection Matrix 
	vpmat = camProj * camView;
	
}