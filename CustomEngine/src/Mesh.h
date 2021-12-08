#pragma once
#include <vector>
#include "VertexData.h"
#include "VertexArrayBuffer.h"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.h"
#include "IndexBuffer.hpp"
#include "Texture.h"

class ShaderProgram; 




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

	void setupMesh();

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

	void loadMesh(const char* filename); 

	// mesh data
	std::vector<VertexData>	  vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;


	void Draw(); 
	void bindBuffers() const; 
	void unbindBuffers() const; 
	unsigned int getIndicesCount() const; 

};

