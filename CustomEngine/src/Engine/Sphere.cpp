#include "Sphere.h"

void Sphere::init(float radius, const glm::vec3& center)
{
	this->clear();

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

			this->vertices.push_back(VertexData({ xyz + center , normal, uvs }));

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

Sphere::Sphere(float radius, const glm::vec3& center)
	: m_radius(radius), m_center(center), 
	dphi(15.0f), drho(15.0f)
{
	
	this->init(m_radius, m_center);
	m_collider = new Collider();
	this->setBbox();
}


Sphere::Sphere()
	: m_radius(1.0f), m_center(glm::vec3(0.0f, 0.0f, 0.0f)),
	dphi(15.0f), drho(15.0f)
{
	this->init(m_radius, m_center);
	m_collider = new Collider();
	this->setBbox();
}