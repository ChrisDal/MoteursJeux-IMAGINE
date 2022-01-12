#include "Mesh.h"
#include "Rendering/ShaderProgram.h"



Mesh::Mesh()
	: m_nVertex(0), indexCount(0), m_primitives(GL_TRIANGLE_STRIP), 
	model_view(glm::mat4(1.0f))
{
	bbox.minbbox = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN); 
	bbox.minbbox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);

	this->vertices = {};
	this->indices = {};
	this->textures = {};
}

Mesh::Mesh(const char* filename)
	: m_nVertex(0), indexCount(0), 
	m_primitives(GL_TRIANGLES), 
	model_view(glm::mat4(1.0f))
{
	this->vertices = {};
	this->indices = {};
	this->textures = {};
	
	// switch according to type 
	int typefile = - 1; 
	if (loadMesh(filename))
	{
		std::cout << "Loaded \n"; 
	}
	else
	{
		std::cout << "Cannot handle this extension file\n" << std::endl; 
	}
	 
}

Mesh::Mesh(std::vector<VertexData> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: m_nVertex(0), indexCount(0), m_primitives(GL_TRIANGLES),
	model_view(glm::mat4(1.0f))
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

void Mesh::initTerrain(const char* filename, int sqrtTerrain)
{
	this->clear();

	// Load heightmap 
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

	if (data == nullptr)
	{
		std::cout << "[STBI] Load Image Error : " << filename << std::endl;
		return; 
	}

	// afficher une surface plane (16*16 sommets) composée de triangles.
	unsigned int sqrtnVertex = std::sqrt(sqrtTerrain);
	unsigned int vertexNumber = sqrtnVertex * sqrtnVertex;

	this->vertices.reserve(vertexNumber);

	// defined subdivision of the plane
	float minPlane = -1.0f;
	float maxPlane = 1.0f;
	float dxy = (maxPlane - minPlane) / float(sqrtnVertex - 1);

	if (nrChannels == 1)
	{
		std::cout << "Only intensity " << std::endl;
	}

	for (unsigned int n = 0; n < sqrtnVertex; n++)
	{
		for (unsigned int m = 0; m < sqrtnVertex; m++)
		{
			//unsigned int k = n*sqrtnVertex + m;
			float textcoordx = static_cast<float>(m) / static_cast<float>(sqrtnVertex - 1);
			float textcoordy = static_cast<float>(n) / static_cast<float>(sqrtnVertex - 1);

			int tx = static_cast<int>(textcoordx * width);
			int ty = static_cast<int>(textcoordy * height);
			int pixValue = data[ty * nrChannels * width + tx * nrChannels];

			float zheight = static_cast<float>(pixValue) / 255.0f;

			this->vertices.push_back(VertexData{ glm::vec3(minPlane + m * dxy, maxPlane - n * dxy,  zheight),
												 glm::vec3(0.0f, 1.0f, 0.0f), 
												 glm::vec2(textcoordx, textcoordy)});

		}
	}
	// Duplication to form GL_TRIANGLE STRIPS is equal to 2* (sqrtnVertex-1) - 1
	unsigned int idxcount = (sqrtnVertex - 1) * (sqrtnVertex * 2) + (2 * (sqrtnVertex - 1) - 1); //Careful update indicesNumber when creating new geometry
	unsigned int tmpCount = (sqrtnVertex - 1) * (sqrtnVertex * 2);

	this->indices.reserve(idxcount);
	std::vector<GLushort> tmpindices;
	tmpindices.reserve(tmpCount);

	for (unsigned int i = 0; i < tmpCount - 1; i += 2)
	{
		tmpindices.push_back(i / 2);
		tmpindices.push_back(i / 2 + sqrtnVertex);
	}

	unsigned int countVertex = 0;  // every N=2*sqrtnVertex samples we duplicate the vertices k and k+1,
	for (unsigned int k = 0; k < tmpCount; k++)
	{
		countVertex++;
		this->indices.push_back(tmpindices[k]);

		if (countVertex % (2 * sqrtnVertex) == 0)
		{
			this->indices.push_back(tmpindices[k]);
			this->indices.push_back(tmpindices[(int)k + 1]);
		}
	}

	// remove data loaded 
	stbi_image_free(data);


	setPrimitives(GL_TRIANGLE_STRIP);

	setupMesh();
}

void Mesh::initQuad()
{
	this->clear(); 
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f,  0.0f, -1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }));  // v0
	this->vertices.push_back(VertexData({ glm::vec3(-1.0f, 0.0f,  1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) })); // v1
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f, 0.0f,  1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) })); // v2
	this->vertices.push_back(VertexData({ glm::vec3( 1.0f, 0.0f, -1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) })); // v3

	this->indices = { 0, 1, 3,
					1, 2, 3 }; 

	setPrimitives(GL_TRIANGLE_STRIP);

	setupMesh();
}

void Mesh::initCapsule()
{
	this->clear();
}

// Init Sphere with normals, UV, 
// radius = 1.0f by default, 
// center at 0 0 0 by default
void Mesh::initSphere()
{
	this->clear();

	// TODO : input parameters
	float radius = 1.0f; 
	const glm::vec3 center(0.0f, 0.0f, 0.0f);


	float dphi   = 15.0f; 
	float drho   = 15.0f; 
	// LAT LON
	float minphi = -90.0f; 
	float maxphi = 90.0f;
	float minrho = -180.0f; 
	float maxrho = 180.0f;

	unsigned int nphi = (unsigned int)((maxphi - minphi) / dphi) + 1; // matplotlib basic 
	unsigned int nrho = (unsigned int)((maxrho - minrho) / drho);

	
	for (unsigned int kphi = 0; kphi < nphi; kphi++)
	{
		for (unsigned int krho = 0; krho < nrho; krho++)
		{
			float phi = glm::radians<float>(minphi + kphi * dphi); 
			float rho = glm::radians<float>(minrho + krho * drho);

			// Not conventionnal : x,z is our moving plan 
			// To have the poles aligned with up camera
			glm::vec3 xyz(radius * glm::cos(phi) * glm::cos(rho),
				          radius * glm::sin(phi), 
						  radius * glm::cos(phi) * glm::sin(rho)
						  );

			glm::vec3 normal(glm::normalize(glm::vec3(xyz - center))); 
			glm::vec2 uvs(kphi / (float)nphi, krho / (float)nrho);

			this->vertices.push_back(VertexData({ xyz , normal, uvs })); 

		}
	}


	//     A -- D 
	//	   | \  |
	//     |  \ |
	//     B -- C
	// Triangle 1 : A B C 
	// Triangle 2 : C D A 

	for (unsigned int kphi = 0; kphi < nphi - 1; kphi++)
	{
		for (unsigned int krho = 0; krho < nrho; krho++)
		{
			// First Triangle
			if (kphi != 0)
			{
				this->indices.push_back(kphi * nrho + krho);
				this->indices.push_back((kphi + 1) * nrho + krho);
				this->indices.push_back((kphi + 1) * nrho + ((krho + 1) % nrho));
			}
			
			// Second Triangle
			if (kphi != nphi - 1)
			{
				// Indices 
				this->indices.push_back((kphi + 1) * nrho + ((krho + 1) % nrho));
				this->indices.push_back(kphi * nrho + ((krho + 1) % nrho));
				this->indices.push_back(kphi * nrho + krho);
			}

		}
	}


	// set pointers
	m_pvertices = &this->vertices[0];
	m_pindices = (GLushort*)&this->indices[0];

	m_nVertex = this->vertices.size();
	indexCount = this->indices.size();

	setPrimitives(GL_TRIANGLES);

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
	this->indices =  {
		 0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
		 4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
		 8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
		12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
		16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
		20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
	};


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

//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
// From TP LoadMesh Created  by Brian Summa
bool Mesh::loadMesh(const char* filename)
{
	this->clear();


	hasUV = false;
	hasNormals = false; 
	bool hasColors = false; 
	
	std::ifstream myfile(filename); 

	// v : vertices 
	// f : faces 
	// vn : normals 
	// vt : uv vertices textures 

	std::vector < glm::vec3 > vert;
	std::vector < unsigned int > ind;
	std::vector < glm::vec2 > uvs;
	std::vector < glm::vec3 > norms;
	std::vector < glm::vec3 > colors; 
	
	if (myfile)
	{
		std::string line; 
		while (getline(myfile, line)) 
		{
			std::istringstream sline(line);
			if ( line[0] == 'v' )
			{
				if (line[1] == 't')
				{
					float x, y;
					char c, c2;
					sline >> c >> c2 >> x >> y;
					uvs.push_back(glm::vec2(x, y));

					hasUV = true; 
				}
				else if (line[1] == 'n')
				{
					float x, y, z;
					char c, c2;
					sline >> c >> c2 >> x >> y >> z;
					norms.push_back(glm::vec3(x, y, z));
					hasNormals = true; 
				}
				else
				{
					std::vector<std::string> tokens;
					const char delimiter = ' '; 
					std::string token; 
					while (getline(sline, token, delimiter))
					{
						tokens.push_back(token);
					}

					// v .f .f .f 
					if (tokens.size() == 4)
					{
						vert.push_back(glm::vec3(std::stof(tokens[1]), 
							std::stof(tokens[2]), std::stof(tokens[3])));
					}
					// v .f .f .f color1 color1 color1 
					else if (tokens.size() == 7)
					{
						vert.push_back(glm::vec3(std::stof(tokens[1]),
												std::stof(tokens[2]), 
												std::stof(tokens[3])));
						// colors normally 
						glm::vec3 color = glm::vec3(std::stoi(tokens[4]),
							std::stoi(tokens[5]),
							std::stoi(tokens[6])); 
						norms.push_back(color / 255.f);

						hasNormals = true;
					}
					
					
				}
				
			}
			else if (line[0] == 'f')
			{
				std::vector<std::string> tokens;
				const char delimiter = ' ';
				std::string token;
				while (getline(sline, token, delimiter))
				{
					tokens.push_back(token);
				}

				std::vector<std::string> tokens2;
				const char delimiter2 = '/';
				std::string token2; 
				for (const std::string& tk : tokens)
				{
					std::istringstream sline2(tk);
					while (getline(sline2, token2, delimiter2))
					{
						tokens2.push_back(token2);
					}

				}

				if (tokens.size() >= tokens2.size())
				{
					
					ind.push_back(std::stoi(tokens[1])-1);
					ind.push_back(std::stoi(tokens[2])-1);
					ind.push_back(std::stoi(tokens[3])-1);
				}
				else
				{
					std::cout << "Split by \/ ";
					ind.push_back(std::stoi(tokens2[1])-1);
					ind.push_back(std::stoi(tokens2[3])-1);
					ind.push_back(std::stoi(tokens2[4])-1);
				}
				
			}
				
			
		}
	}

	
	std::cout << "Total: " << vert.size() << " vertices\n";
	std::cout << "Total: " << norms.size() << " normals\n";


	
	for (unsigned int i = 0; i < vert.size(); i++) {

		VertexData vdata; 
		if (hasNormals && hasUV) {
			vdata = { vert[i], norms[i], uvs[i] };
		}
		else if (hasNormals && !hasUV)
		{
			vdata = { vert[i], norms[i], glm::vec2(0.0, 0.0) };
		}
		else if ( !hasNormals && !hasUV)
		{
			vdata = { vert[i],glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0, 0.0) };
		}
		else if (!hasNormals && hasUV)
		{
			vdata = { vert[i],glm::vec3(1.0f, 0.0f, 0.0f), uvs[i] };
		}
		
		if (vert[i].x < bbox.minbbox.x) { bbox.minbbox.x = vert[i].x; }
		if (vert[i].y < bbox.minbbox.y) { bbox.minbbox.y = vert[i].y; }
		if (vert[i].z < bbox.minbbox.z) { bbox.minbbox.z = vert[i].z; }
		if (vert[i].x > bbox.maxbbox.x) { bbox.maxbbox.x = vert[i].x; }
		if (vert[i].y > bbox.maxbbox.y) { bbox.maxbbox.y = vert[i].y; }
		if (vert[i].z > bbox.maxbbox.z) { bbox.maxbbox.z = vert[i].z; }

		this->vertices.push_back(vdata); 
		
	}

	bbox.center = bbox.minbbox + 0.5f * (bbox.maxbbox - bbox.minbbox);
	float scale = std::max(bbox.maxbbox.x - bbox.minbbox.x, 
							bbox.maxbbox.y - bbox.minbbox.y);
	this->model_view = glm::translate(glm::mat4(1.0f), -bbox.center);
	// Make the extents 0-1
	this->model_view = glm::scale(this->model_view, glm::vec3(1.0 / scale,
										1.0 / scale,
										1.0 / scale));  //Orient Model About Center
	this->indices = ind; 

	setPrimitives(GL_TRIANGLES);
	setupMesh();

	return true; 
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
	this->bbox.maxbbox = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);

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
