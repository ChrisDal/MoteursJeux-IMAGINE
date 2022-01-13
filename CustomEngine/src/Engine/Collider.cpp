#include "Collider.h"

Collider::Collider()
{
	invalidBbox(); 
}

void Collider::processBbox(const std::vector<VertexData>& vertices)
{
	invalidBbox();

	for (const VertexData& pv : vertices)
	{
		for (unsigned int k = 0; k < 3; k++)
		{
			if (pv.positions[k] < m_bbox.minbbox[k]) {
				m_bbox.minbbox[k] = pv.positions[k];
			}

			if (pv.positions[k] > m_bbox.maxbbox[k]) {
				m_bbox.maxbbox[k] = pv.positions[k];
			}
		}

	}

	m_bbox.center = (m_bbox.minbbox + m_bbox.maxbbox) / 2.0f; 
}

void Collider::invalidBbox()
{
	m_bbox.minbbox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_bbox.maxbbox = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
	m_bbox.center = glm::vec3(0.0f, 0.0f, 0.0f);
}
