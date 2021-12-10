#include "Mesh.h"
#include "Rendering/ShaderProgram.h"


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

void Mesh::initPlane()
{
	this->vertices.clear();
	this->indices.clear();
	this->textures.clear();

	this->vertices = {
		// positions          // normals           // texture coords
		VertexData({glm::vec3(0.5f,  0.5f, 0.0f),    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}),    // top right
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

void Mesh::initCube()
{
	this->vertices.clear();
	this->indices.clear();
	this->textures.clear();

	// Vertex data for face 0
	m_vertices.push_back({ glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.0f, 0.0f) });  // v0
	m_vertices.push_back({ glm::vec3(1.0f, -1.0f,  1.0f),  glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.33f, 0.0f) }); // v1
	m_vertices.push_back({ glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.0f, 0.5f) }); // v2
	m_vertices.push_back({ glm::vec3(1.0f,  1.0f,  1.0f),  glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.33f, 0.5f) }); // v3

	// Vertex data for face 1
	m_vertices.push_back({ glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.0f, 0.5f) }); // v4
	m_vertices.push_back({ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.33f, 0.5f) }); // v5
	m_vertices.push_back({ glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.0f, 1.0f) });  // v6
	m_vertices.push_back({ glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.33f, 1.0f) }); // v7

	// Vertex data for face 2
	m_vertices.push_back({ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(0.66f, 0.5f) }); // v8
	m_vertices.push_back({ glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(1.0f, 0.5f) }); // v9
	m_vertices.push_back({ glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(0.66f, 1.0f) }); // v10
	m_vertices.push_back({ glm::vec3(-1.0f,  1.0f, -1.0f),glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) }); // v11

	// Vertex data for face 3
	m_vertices.push_back({ glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(1.0f, 0.0, 0.0f),glm::vec2(0.66f, 0.0f) }); // v12
	m_vertices.push_back({ glm::vec3(-1.0f, -1.0f,  1.0f),glm::vec3(1.0f, 0.0, 0.0f),glm::vec2(1.0f, 0.0f) }); // v13
	m_vertices.push_back({ glm::vec3(-1.0f,  1.0f, -1.0f),glm::vec3(1.0f, 0.0, 0.0f),glm::vec2(0.66f, 0.5f) }); // v14
	m_vertices.push_back({ glm::vec3(-1.0f,  1.0f,  1.0f),glm::vec3(1.0f, 0.0, 0.0f),glm::vec2(1.0f, 0.5f) }); // v15

	// Vertex data for face 4
	m_vertices.push_back({ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0, 0.0f,1.0f),glm::vec2(0.33f, 0.0f) }); // v16
	m_vertices.push_back({ glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec3(0.0, 0.0f,1.0f),glm::vec2(0.66f, 0.0f) }); // v17
	m_vertices.push_back({ glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0, 0.0f,1.0f),glm::vec2(0.33f, 0.5f) }); // v18
	m_vertices.push_back({ glm::vec3(1.0f, -1.0f,  1.0f),  glm::vec3(0.0, 0.0f,1.0f),glm::vec2(0.66f, 0.5f) }); // v19

	// Vertex data for face 5
	m_vertices.push_back({ glm::vec3(-1.0f,  1.0f,  1.0f),glm::vec3(0.0, 1.0f, 0.0f),glm::vec2(0.33f, 0.5f) }); // v20
	m_vertices.push_back({ glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0, 1.0f, 0.0f),glm::vec2(0.66f, 0.5f) }); // v21
	m_vertices.push_back({ glm::vec3(-1.0f,  1.0f, -1.0f),glm::vec3(0.0, 1.0f, 0.0f),glm::vec2(0.33f, 1.0f) }); // v22
	m_vertices.push_back({ glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0, 1.0f, 0.0f),glm::vec2(0.66f, 1.0f) }); // v23

	m_nVertex = m_vertices.size();



	// Sommets
	int ki = 0;
	for (unsigned int k = 0; k < m_nVertex; k++)
	{
		m_indices.push_back(k);
		ki++;
		if (((k + 1) % 4 == 0) && k > 0)
		{
			m_indices.push_back(k);
			m_indices.push_back(k + 1);
			ki++;
			ki++;
		}
	}

	// set pointers
	m_pvertices = &m_vertices[0];
	m_pindices = &m_indices[0];

	m_nVertex = m_vertices.size();
	indexCount = m_indices.size();

}


void Mesh::loadMesh(const char* filename)
{
	this->vertices.clear();
	this->indices.clear();
	this->textures.clear();
}

void Mesh::clear()
{
	m_vertices.clear(); 
	m_bbox.clear(); 
	m_indices.clear(); 

	m_nVertex = 0; 
	indexCount = 0; 
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

void Mesh::setShaderProg(ShaderProgram* shaderprog)
{
	m_shader = shaderprog; 
}

bool Mesh::isShaderSet() const
{
	return m_shader != nullptr;
}

bool Mesh::useShader() const
{
	if (m_shader != nullptr)
	{
		m_shader->link(); 
		return true; 
	}
	else
	{
		return false; 
	}
}
