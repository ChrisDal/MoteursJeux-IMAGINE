#pragma once
#include <vector>
#include "Vdata.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{

private: 
	//  render data
	unsigned int VAO; 
	unsigned int VBO; 
	unsigned int EBO;

	void setupMesh();


public:

	Mesh();
	Mesh(const char* filename); 
	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures); 
	~Mesh(); 

	void initSphere(); 
	void initCube();
	void initCapsule();

	void loadMesh(const char* filename); 
	void setMesh(VertexData* data); 

	// mesh data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;

	void Draw(); 

};

