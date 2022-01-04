#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "Rendering/VertexData.h"
#include "Rendering/VertexArrayBuffer.h"
#include "Rendering/VertexBuffer.hpp"
#include "Rendering/VertexBufferLayout.h"
#include "Rendering/IndexBuffer.hpp"
#include "Rendering/Texture.h"
#include "Rendering/ShaderProgram.h"


namespace SpaceEngine {

	struct boundingBox
	{
		glm::vec3 minbbox; 
		glm::vec3 maxbbox; 
		glm::vec3 center; 
	};
	
	// Visualise Bounding Box 
	

}

class Mesh
{

private: 
	//  render data
	VertexArrayBuffer* m_vao; 
	VertexBuffer* m_vbo;
	IndexBuffer* m_ibo; 
	VertexBufferLayout m_layout; 

	// shader 
	ShaderProgram* m_shader = nullptr; // if not 

	//  Triangle or Triangle Strip 
	GLenum m_primitives;

	// Data to draw - Cube geometry
	std::vector<Texture>    m_textures; 
	// Size count 
	unsigned int m_nVertex;
	unsigned int indexCount;

	VertexData* m_pvertices;
	GLushort* m_pindices;

	void setupMesh();

	bool hasUV = false;


protected: 

	// mesh data
	std::vector<VertexData>	  vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;
	SpaceEngine::boundingBox  bbox;   // Bounding box of mesh 

	// Debug 
	void debugMesh() const; 

public:

	Mesh();
	Mesh(const char* filename); 
	Mesh(std::vector<VertexData> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures); 

	~Mesh(); 

	void initSphere(); 
	void initCube();
	void initCapsule();
	void initPlane(); 

	bool loadMesh(const char* filename); 
	void clear(); 

	// Bounding Box 
	void setBbox(); 
	SpaceEngine::boundingBox getBbox() const {
		return bbox;
	};

	unsigned int getNumTri() { return vertices.size() / 3; }

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

};

