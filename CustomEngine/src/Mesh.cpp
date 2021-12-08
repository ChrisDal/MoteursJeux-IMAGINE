#include "Mesh.h"


void Mesh::setupMesh()
{
	m_vao = new VertexArrayBuffer(); 
	m_vao->bind();
	// vertex buffer 
	m_vbo = new VertexBuffer(this->vertices.data(), this->vertices.size() * sizeof(this->vertices[0]));
	// indices buffer 
	m_ibo = new IndexBuffer(this->indices.data(), this->indices.size()); 
	// Layout
	m_layout = VertexBufferLayout();
	m_layout.Push<float>(3); // layout position 
	m_layout.Push<float>(3); // layout normals
	m_layout.Push<float>(2); // layout texture

	// Add Layout to VAO 
	m_vao->addBuffer(m_vbo, &m_layout);

}

Mesh::Mesh()
{
}

Mesh::Mesh(const char* filename)
{
}

Mesh::Mesh(std::vector<VertexData> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices; 
	this->indices = indices; 
	this->textures = textures; 

	this->setupMesh(); 
}

Mesh::~Mesh()
{
	if (m_vao != nullptr) { delete m_vao; }
	if (m_vbo != nullptr) { delete m_vbo; }
	if (m_ibo != nullptr) { delete m_ibo; }
}

void Mesh::initSphere()
{
	this->vertices.clear();
}

void Mesh::initCube()
{
	this->vertices.clear();
	this->indices.clear();
	this->textures.clear();

	this->vertices = {
		// positions          // normals           // texture coords
		VertexData({glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}),    // top right
		VertexData({glm::vec3( 0.5f, -0.5f, 0.0f),   glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}),   // bottom right
		VertexData({glm::vec3(-0.5f, -0.5f, 0.0f),   glm::vec3(0.0f, 0.0f, 1.0f),   glm::vec2(0.0f, 0.0f)}),  // bottom left
		VertexData({glm::vec3(-0.5f,  0.5f, 0.0f),   glm::vec3(1.0f, 1.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}),  // top left 
	};

	this->indices = { 0, 1, 3,
					1, 2, 3 }; 

	
}

void Mesh::initCapsule()
{
	this->vertices.clear();
	this->indices.clear();
	this->textures.clear();
}

void Mesh::loadMesh(const char* filename)
{
	this->vertices.clear();
	this->indices.clear();
	this->textures.clear();
}



void Mesh::Draw()
{
}

void Mesh::bindBuffers() const
{
	m_vao->bind(); 
	m_ibo->bind(); 
}

void Mesh::unbindBuffers() const
{
	m_vao->unbind(); 
	m_ibo->unbind(); 
}

unsigned int Mesh::getIndicesCount() const
{
	return m_ibo->getCount();
}
