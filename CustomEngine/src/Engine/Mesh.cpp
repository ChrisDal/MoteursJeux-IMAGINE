#include "Mesh.h"
#include "Rendering/ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>


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
	: m_nVertex(0), indexCount(0), m_primitives(GL_TRIANGLE_STRIP)
{
	bbox.minbbox = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN); 
	bbox.minbbox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);

	this->vertices = {};
	this->indices = {};
	this->textures = {};
}

Mesh::Mesh(const char* filename)
	: m_nVertex(0), indexCount(0), m_primitives(GL_TRIANGLES)
{
	this->vertices = {};
	this->indices = {};
	this->textures = {};
	
	// switch according to type 
	int typefile = - 1; 
	if (typefile == 0)
	{
		// obj == Load obj 
	}
	else if (typefile == 1)
	{
		// off == load off
	}
	else
	{
		std::cout << "Cannot handle this extension file\n" << std::endl; 
	}
	 
}

Mesh::Mesh(std::vector<VertexData> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: m_nVertex(0), indexCount(0), m_primitives(GL_TRIANGLES)
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



void Mesh::initPlane()
{
	this->clear();

	this->vertices = {
		// positions          // normals           // texture coords
		VertexData({glm::vec3(0.5f,  0.5f, 0.0f),    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}),    // top right
		VertexData({glm::vec3( 0.5f, -0.5f, 0.0f),   glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}),   // bottom right
		VertexData({glm::vec3(-0.5f, -0.5f, 0.0f),   glm::vec3(0.0f, 0.0f, 1.0f),   glm::vec2(0.0f, 0.0f)}),  // bottom left
		VertexData({glm::vec3(-0.5f,  0.5f, 0.0f),   glm::vec3(1.0f, 1.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}),  // top left 
	};

	this->indices = { 0, 1, 3, 
					1, 2, 3 }; 

	setPrimitives(GL_TRIANGLE_STRIP);

	setupMesh();
}

void Mesh::initCapsule()
{
	this->clear();
}

// ---------------------------------------
// init Sphere data
void Mesh::initSphere()
{
	this->clear();

	float radius = 1.0f; 
	float dphi   = 1.0f; 
	float drho  = 1.0f; 

	float minphi = -90.0f; 
	float minrho = -180.0f; 
	float maxphi = 90.0f;
	float maxrho = 180.0f;

	unsigned int nphi = (unsigned int)((maxphi - minphi) / dphi);
	unsigned int nrho = (unsigned int)((maxrho - minrho) / drho);

	const glm::vec3 center(0.0f, 0.0f, 0.0f); 


	
	for (unsigned int kphi = 0; kphi < nphi; kphi++)
	{
		for (unsigned int krho = 0; krho < nrho; krho++)
		{
			float phi = glm::radians<float>(minphi + kphi * dphi); 
			float rho = glm::radians<float>(minrho + krho * drho);

			glm::vec3 xyz(radius * glm::cos(phi) * glm::sin(rho),
							radius * glm::cos(phi) * glm::cos(rho),
							radius * glm::sin(phi));

			glm::vec3 normal(glm::vec3(xyz - center)); 
			glm::vec2 uvs(0.0f, 0.0f); 

			this->vertices.push_back(VertexData({ xyz , normal, uvs })); 

			this->indices.push_back(kphi* nrho + krho);
		}
	}


	// set pointers
	m_pvertices = &this->vertices[0];
	m_pindices = (GLushort*)&this->indices[0];

	m_nVertex = this->vertices.size();
	indexCount = this->indices.size();

	setPrimitives(GL_POINTS);

	setupMesh();
}


void Mesh::initCube()
{
	this->clear();

	// positions          // normals           // texture coords

	// Vertex data for face 0
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.0f, 0.0f) }));  // v0
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.33f, 0.0f) })); // v1
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.0f, 0.5f) })); // v2
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.33f, 0.5f) })); // v3

	// Vertex data for face 1
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.0f, 0.5f) })); // v4
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.33f, 0.5f) })); // v5
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.0f, 1.0f) }));  // v6
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.33f, 1.0f) })); // v7

	// Vertex data for face 2
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(0.66f, 0.5f) })); // v8
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(1.0f, 0.5f) })); // v9
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(0.66f, 1.0f) })); // v10
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) })); // v11

	// Vertex data for face 3
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.66f, 0.0f) })); // v12
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(1.0f, 0.0f) })); // v13
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.66f, 0.5f) })); // v14
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(1.0f, 0.5f) })); // v15

	// Vertex data for face 4
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0, 0.0f,1.0f), glm::vec2(0.33f, 0.0f) })); // v16
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0, 0.0f,1.0f), glm::vec2(0.66f, 0.0f) })); // v17
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0, 0.0f,1.0f), glm::vec2(0.33f, 0.5f) })); // v18
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0, 0.0f,1.0f), glm::vec2(0.66f, 0.5f) })); // v19

	// Vertex data for face 5
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f,  1.0f,  1.0f),glm::vec3(0.0, 1.0f, 0.0f),glm::vec2(0.33f, 0.5f) })); // v20
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0, 1.0f, 0.0f),glm::vec2(0.66f, 0.5f) })); // v21
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f,  1.0f, -1.0f),glm::vec3(0.0, 1.0f, 0.0f),glm::vec2(0.33f, 1.0f) })); // v22
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0, 1.0f, 0.0f),glm::vec2(0.66f, 1.0f) })); // v23

	m_nVertex = this->vertices.size();

	// Sommets
	int ki = 0;
	for (unsigned int k = 0; k < m_nVertex; k++)
	{
		this->indices.push_back(k);
		ki++;
		if (((k + 1) % 4 == 0) && k > 0)
		{
			this->indices.push_back(k);
			this->indices.push_back(k + 1);
			ki++;
			ki++;
		}
	}

	this->indices.push_back(m_nVertex); 

#if 0
	debugMesh(); 
#endif 
	

	// set pointers
	m_pvertices = &this->vertices[0];
	m_pindices = (GLushort*)&this->indices[0];

	m_nVertex = this->vertices.size();
	indexCount = this->indices.size();
	
	setPrimitives(GL_TRIANGLE_STRIP);
	setupMesh();

}

// Display Debug informations 
void Mesh::debugMesh() const
{
	std::cout << "Cube Indices : ";
	for (const unsigned int& id : this->indices)
	{
		std::cout << id << ", ";
	}

	std::cout << std::endl;
}


void Mesh::loadMesh(const char* filename)
{
	this->clear();


	// Load mesh file
}

void Mesh::clear()
{
	if (!this->vertices.empty())
	{
		this->vertices.clear();
	}
	if (!this->indices.empty())
	{
		this->indices.clear();
	}

	if (!this->textures.empty()){
		this->textures.clear();
	}

	m_nVertex = 0;
	indexCount = 0;

	this->bbox.minbbox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	this->bbox.maxbbox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);

}

void Mesh::setBbox()
{
	bbox.minbbox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	bbox.maxbbox = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);

	for (const VertexData& pv : this->vertices)
	{
		for (unsigned int k = 0; k < 3; k++)
		{
			if (pv.positions[k] < bbox.minbbox[k]) {
				bbox.minbbox[k] = pv.positions[k];
			}

			if (pv.positions[k] > bbox.maxbbox[k]) {
				bbox.maxbbox[k] = pv.positions[k];
			}
		}

	}
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
