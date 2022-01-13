#pragma once
#include <vector>
#include "Rendering/VertexData.h"
#include "GeomPhysics.h"


class Mesh; 

class Collider
{
private:

	SpaceEngine::boundingBox m_bbox; 
	Mesh* m_parent; 

public: 
	Collider(); 
	void attach(Mesh* parent) { m_parent = parent; }

	// Rectangle englobant 
	void processBbox(const std::vector<VertexData>& vertices);

	SpaceEngine::boundingBox getBbox() const { return m_bbox;  }
	SpaceEngine::boundingBox* getpBbox() { return &m_bbox; }
	void invalidBbox(); 

	//brute Force
	inline bool intersect(const Collider& other) {
		bool onX = (m_bbox.minbbox.x <= other.m_bbox.minbbox.x &&
					other.m_bbox.minbbox.x <= m_bbox.maxbbox.x) ||
				   (other.m_bbox.minbbox.x <= m_bbox.maxbbox.x &&
					m_bbox.maxbbox.x <= other.m_bbox.maxbbox.x); 
		bool onY = (m_bbox.minbbox.y <= other.m_bbox.minbbox.y &&
					other.m_bbox.minbbox.y <= m_bbox.maxbbox.y) ||
				   (other.m_bbox.minbbox.y <= m_bbox.maxbbox.y &&
				    m_bbox.maxbbox.y <= other.m_bbox.maxbbox.y);
		bool onZ = (m_bbox.minbbox.z <= other.m_bbox.minbbox.z &&
					other.m_bbox.minbbox.z <= m_bbox.maxbbox.z) ||
				   (other.m_bbox.minbbox.z <= m_bbox.maxbbox.z &&
					m_bbox.maxbbox.z <= other.m_bbox.maxbbox.z);

		return onX && onY && onZ; 
	}
	
}; 