#pragma once
#include <vector>
#include "Rendering/VertexData.h"
#include "Rendering/VertexArrayBuffer.h"
#include "Rendering/VertexBuffer.hpp"
#include "Rendering/VertexBufferLayout.h"
#include "Rendering/IndexBuffer.hpp"
#include "Rendering/Texture.h"
#include "Rendering/ShaderProgram.h"

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

	// Data to draw - Cube geometry
	std::vector<VertexData> m_vertices;
	std::vector<glm::vec3>  m_bbox;
	std::vector<GLushort>   m_indices;
	std::vector<Texture>    m_textures; 
	unsigned int m_nVertex;
	unsigned int indexCount;

	VertexData* m_pvertices;
	GLushort* m_pindices;

	void setupMesh();


protected: 

	// OpenGL Render data
	unsigned int getNVertex() const { return  m_nVertex; }
	unsigned int getIndexCount() const { return indexCount; }
	VertexData* getVertices() { return m_vertices.data(); }
	GLushort* getIndices() { return  m_indices.data(); }

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

	void loadMesh(const char* filename); 
	void clear(); 

	// mesh data
	std::vector<VertexData>	  vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;


	void Draw(); 
	void bindBuffers() const; 
	void unbindBuffers() const; 
	unsigned int getIndicesCount() const; 

	// Rendering 
	void setShaderProg(ShaderProgram* shaderprog); 
	bool isShaderSet() const; 
	bool useShader() const; 
	ShaderProgram* getShader() { return m_shader;  }

};

