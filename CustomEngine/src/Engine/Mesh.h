#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
// GLM 
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


// Rendering 
#include "Rendering/VertexData.h"
#include "Rendering/VertexArrayBuffer.h"
#include "Rendering/VertexBuffer.hpp"
#include "Rendering/VertexBufferLayout.h"
#include "Rendering/IndexBuffer.hpp"
#include "Rendering/Texture.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Material.h"

// Geometry
#include "GeomPhysics.h"
#include "Collider.h"



class Mesh
{

protected: 
	//  render data
	VertexArrayBuffer* m_vao; 
	VertexBuffer* m_vbo;
	IndexBuffer* m_ibo; 
	VertexBufferLayout m_layout; 

	// shader 
	ShaderProgram* m_shader = nullptr; // if not 

	// Material 
	Material* m_material; 
	glm::vec4 m_color; 
	

	//  Triangle or Triangle Strip 
	GLenum m_primitives;

	// Data to draw - Cube geometry
	std::vector<Texture>    m_textures; 
	// Size count 
	unsigned int m_nVertex;
	unsigned int indexCount;

	VertexData* m_pvertices;
	GLushort* m_pindices;

	bool hasUV = false;
	bool hasNormals = false; 
	glm::mat4 model_view; 

	// mesh data
	std::vector<VertexData>	  vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;

	// Bounding Box if 
	Collider* m_collider; 
	//SpaceEngine::boundingBox  bbox;   // Bounding box of mesh 

	// Mesh utils Functions 
	void debugMesh() const; 
	void setupMesh();

public:

	Mesh();
	Mesh(const char* filename); 
	Mesh(std::vector<VertexData> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures); 

	~Mesh(); 

	void initSphere(float radius=1.0f); 
	void initCube();
	void initCapsule(float radius=0.5f, float distance=1.0f);
	void initPlane() {};
	void initQuad(); 
	void initTerrain(const char* filename, int sqrtTerrain=16);


	bool loadMesh(const char* filename); 
	void clear(); 

	// Collider Box  
	SpaceEngine::boundingBox getBbox() const { return m_collider->getBbox() ;  }
	void setBbox(Collider collider); 
	void setBbox(); 

	unsigned int getNumTri() { return (int)vertices.size() / 3; }

	// Uniform 
	glm::mat4 getModelView() const { return model_view; }

	// OpenGL Render data
	unsigned int getNVertex() const { return  m_nVertex; }
	unsigned int getIndexCount() const { return indexCount; }
	VertexData* getVertices() { return vertices.data(); }
	VertexData getVertices(int index) { return vertices[index];  } // no modifyable
	unsigned* getIndices() { return  indices.data(); }


	void Draw(); 
	void bindBuffers() const; 
	void unbindBuffers() const; 
	unsigned int getIndicesCount() const; 

	// Rendering 
	void setShaderProg(ShaderProgram* shaderprog); 
	bool isShaderSet() const; 
	bool useShader() const; 
	ShaderProgram* getShader() { return m_shader;  }
	inline GLenum getPrimitives() const { return m_primitives;  }
	inline void setPrimitives(GLenum mode) { m_primitives = mode; }


	// Albedo and color 
	void setColor(const glm::vec4& color); 
	void setColor(float r, float g, float b, float a); 
	glm::vec4 getColor() const { return m_color;  }

	// Material 
	inline bool hasMaterial() const { return m_material != nullptr;  }
	void setMaterial(const glm::vec3& amb, const glm::vec3& diff, 
					const glm::vec3& spec, const float& shininess); 
	Material* getMat() const { return m_material; }

	static const glm::vec4 basicColor;

};

