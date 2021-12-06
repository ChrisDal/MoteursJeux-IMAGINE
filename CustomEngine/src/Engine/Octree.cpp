#include "Octree.hpp"

/* This is a student octree implementation.
 * 	// Octree is made of 8 cubes
    //        G ---  H
    // 	   E ---- F	 |
    // 	   |  C --|- D
    //     A ---- B
 *
 * Their position are displayed above.
 * Depth of the octree can be retrieved from Octree::depth
 * A cube is given by a lower left (near) point
 * and a upper right (far) point.
 ** m_point is the point contained by the region
 * If null and no children : region is empty
 * If null and children : go to sub region
 * If not null and no children : there is only one point
 * If not null and children : error
 *
*/

int Octree::depth = 0;

Octree::Octree(const Point3D& bottomnearleft, const Point3D& topfarright, short int tag)
    : m_tag(tag), m_depth(Octree::depth)
{
    // set limits 
    limit_a = new Point3D(bottomnearleft);
    limit_b = new Point3D(topfarright);

    setTagString();

    std::cout << "Octree=" << this->m_stag.c_str() << " Limits = " << *limit_a << "&" << *limit_b;

}


Octree::Octree(float xa, float ya, float za, float xb, float yb, float zb, short int tag)
    : m_tag(tag), m_depth(Octree::depth)
{
    // set limits 
    limit_a = new Point3D(xa, ya, za);
    limit_b = new Point3D(xb, yb, zb);

    // set tag string
    setTagString();

    std::cout << "Octree=" << this->m_stag.c_str() << " Limits = " << *limit_a << "&" << *limit_b;


}

// set Tag String : D DepthNumber Letter cf SplitSpace

void Octree::setTagString()
{
    // Depth 
    m_stag = "D";
    m_stag += std::to_string(m_depth);

    char c = m_tag + 65;
    m_stag += c;
}

Octree::~Octree()
{
    if (limit_a != nullptr)
    {
        delete limit_a;
        delete limit_b;
    }

    if (m_point != nullptr)
    {
        delete m_point;
    }

    if (m_children.size() > 0)
    {
        for (unsigned int k = 0; k < m_children.size(); k++) {
            delete m_children[k];
        }

    }

    std::cout << "Destructor Octree " << getStag().c_str();
}

Point3D Octree::getMid(const Point3D& pa, const Point3D& pb)
{
    return (pa + pb) / 2.0f;
}

void Octree::splitSpace(const Point3D& pa, const Point3D& pb)
{
    // valid pa and pb 

    if ((pa.x >= pb.x) || (pa.y >= pb.y) || (pa.z >= pb.z))
    {
        std::cout << "Stop split process : Point3D pa pb not valid.";
        std::cout << "PA" << pa << "PB " << pb;
        return;
    }

    // center of the cube 
    Point3D center = (pa + pb) / 2.0f;
    Point3D dxyz = { float(pb.x - pa.x) / 2.0f,
                    float(pb.y - pa.y) / 2.0f,
                    float(pb.z - pa.z) / 2.0f };

    // dxyz is the dx /2.f
    Point3D dx = Point3D(dxyz.x, 0.0f, 0.0f);
    Point3D dy = Point3D(0.0f, dxyz.y, 0.0f);
    Point3D dz = Point3D(0.0f, 0.0f, dxyz.z);

    // Cube is made of 8 cubes 
    //        G ---  H
    // 	   E ---- F	 | 	
    // 	   |  C --|- D 
    //     A ---- B 


    Point3D A[2] = { pa,  center };
    Point3D B[2] = { pa + dx,  center + dx };
    Point3D C[2] = { pa + dy,  center + dy };
    Point3D D[2] = { center - dz,  center + dx + dy };
    Point3D E[2] = { pa + dz,  center + dz };
    Point3D F[2] = { center - dy,  center + dx + dz };
    Point3D G[2] = { center - dx,  center + dy + dz };
    Point3D H[2] = { center ,  pb };

    if (m_children.size() > 0)
    {
        for (unsigned int k = 0; k < m_children.size(); k++)
        {
            delete m_children[k];
        }
    }

    // each time we split space, increase depth
    Octree::depth++;

    m_children.push_back(new Octree(A[0], A[1], 0));
    m_children.push_back(new Octree(B[0], B[1], 1));
    m_children.push_back(new Octree(C[0], C[1], 2));
    m_children.push_back(new Octree(D[0], D[1], 3));
    m_children.push_back(new Octree(E[0], E[1], 4));
    m_children.push_back(new Octree(F[0], F[1], 5));
    m_children.push_back(new Octree(G[0], G[1], 6));
    m_children.push_back(new Octree(H[0], H[1], 7));

    for (Octree* c : m_children)
    {
        c->setParent(this);
    }

}

bool Octree::insideBounds(const Point3D& p) const
{

    if ((p.x < this->limit_a->x) && (p.y < limit_a->y) && (p.z < limit_a->z))
    {
        return false;
    }

    if ((p.x > limit_b->x) && (p.y > limit_b->y) && (p.z > limit_b->z))
    {
        return false;
    }

    return (p.x >= limit_a->x && p.x < limit_b->x) && (p.y >= limit_a->y && p.y < limit_b->y) && (p.z >= limit_a->z && p.z < limit_b->z);

}


void Octree::insert(float xi, float yi, float zi)
{

    Point3D p_i = Point3D(xi, yi, zi);
    this->insert(p_i);
}


void Octree::insert(const Point3D& p)
{
    if (!this->insideBounds(p)) {
        std::cout << "Outside Bounds" << p;
        return;
    }

    if (find(p))
    {
        std::cout << "Point" << p << " already exists in mesh";
        return;
    }

    // if just a box for a point 
    // move the point to a smaller box 
    if (m_point != nullptr)
    {
        Point3D old_point(*m_point);

        delete m_point;
        m_point = nullptr;

        this->insert(old_point);
    }

    if (m_children.size() < 8)
    {
        //std::cout << "No children: SplitSpace" ;
        this->splitSpace(*(this->limit_a), *(this->limit_b));
    }


    Octree* foundOctree = nullptr;
    for (Octree* oct : getChildren())
    {
        if (oct->insideBounds(p)) {
            foundOctree = oct;
            std::cout << "Found a box to put our point : " << foundOctree->getStag().c_str();
            break;
        }
    }

    if (foundOctree == nullptr)
    {
        std::cout << "STOP. No valid Octree found for point P" << p;
        return;
    }

    // Insert in the right position in oct 
    if ((foundOctree->getMpoint() == nullptr) && foundOctree->getChildren().empty())
    {
        std::cout << "Set m_point" << p << " is Found Octree : " << foundOctree << " Tag=" << foundOctree->getStag().c_str();
        foundOctree->setMpoint(new Point3D(p));
    }
    else // need Split 
    {
        //std::cout << "m_point already set or region, split again" << foundOctree->getStag().c_str() ;
        foundOctree->insert(p);
    }


}


bool Octree::find(const Point3D& p)
{
    if (!this->insideBounds(p))
    {
        return false;
    }

    if (m_point != nullptr && m_children.empty())
    {
        bool found = (p.x == m_point->x) && (p.y == m_point->y) && (p.z == m_point->z);

        if (found) {
            std::cout << "Found in Octree" << m_stag.c_str();
        }

        return found;
    }


    Octree* foct = nullptr;
    for (Octree* child : m_children)
    {
        if (child->insideBounds(p))
        {
            foct = child;
            break;
        }
    }

    if (foct != nullptr)
    {
        return foct->find(p);
    }
    else
    {
        return false;
    }
}

bool Octree::find(float xi, float yi, float zi)
{
    Point3D p_i = Point3D(xi, yi, zi);
    return this->find(p_i);
}

// Query
Point3D* Octree::findclosest(const Point3D& p) const
{
    // outside bounds -> doesnt find
    if (!this->insideBounds(p))
    {
        return nullptr;
    }

    if (this->isRegionEmpty())
    {
        return m_parent->getMpoint();
    }

    // Octree ends here
    if (m_point != nullptr && m_children.empty())
    {
        std::cout << "Found Closest in Octree" << m_stag.c_str();
        return m_point;
    }

    // Iterate through children
    Octree* foct = nullptr;
    for (Octree* child : m_children)
    {
        if (child->insideBounds(p))
        {
            foct = child;
            break;
        }
    }

    if (foct != nullptr)
    {
        return foct->findclosest(p);
    }
    else
    {
        return m_point; // m_point == nullptr
    }


}


Point3D* Octree::findclosestPoint(const Point3D& p, float& distance, Octree* minOct)
{

    // outside bounds -> doesnt find
    if (!this->insideBounds(p))
    {
        return nullptr;
    }

    if (this->isRegionEmpty())
    {
        return m_parent->findclosestPoint(p, distance, minOct);
    }

    // Octree ends here
    if (m_point != nullptr && m_children.empty())
    {
        std::cout << "Found Closest in Octree" << m_stag.c_str();
        return m_point;
    }

    // Iterate through children
    Octree* foct = nullptr;
    for (Octree* child : m_children)
    {
        if (child->insideBounds(p) && (!child->isRegionEmpty()))
        {
            foct = child;
            break;
        }
    }

    if (foct != nullptr)
    {
        return foct->findclosest(p);
    }
    else
    {
        // iterate through children 
        getMinOctreeDist(m_children, distance, minOct, p);
        return minOct->getMpoint();
    }

}
// Query sub octant 
// Query
Octree* Octree::findclosestOctree(const Point3D& p)
{
    // outside bounds -> doesnt find
    if (!this->insideBounds(p))
    {
        return nullptr;
    }

    if (this->isRegionEmpty())
    {
        return this;
    }

    // Octree ends here
    if (m_point != nullptr && m_children.empty())
    {
        std::cout << "Found Closest in Octree" << m_stag.c_str();
        return this;
    }

    // Iterate through children
    Octree* foct = nullptr;
    for (Octree* child : m_children)
    {
        if (child->insideBounds(p))
        {
            foct = child;
            break;
        }
    }

    if (foct != nullptr)
    {
        return foct->findclosestOctree(p);
    }
    else
    {
        return m_parent; // m_point == nullptr
    }

}

// return true if the region is empty
// we automatically split by  8
bool Octree::isRegionEmpty() const
{
    return (m_point == nullptr) && (m_children.size() == 0);
}

void Octree::setParent(Octree* parent) {
    m_parent = parent;
}

// Return a pointer to a Point3D,  bounding of the octree
// if isA return limit_a 
// else return limit_b 
Point3D* Octree::getLimit(bool isA)
{
    if (isA) {
        return limit_a;
    }

    return limit_b;

}

std::vector<Octree*> Octree::getSiblings() const
{
    if (m_parent == nullptr) {
        return std::vector<Octree*>();
    }

    return m_parent->getChildren();

}


float Octree::getDistance(const Point3D& p) const
{
    float distance = FLT_MAX;

    if (m_point == nullptr)
    {
        return distance;
    }

    distance = (float)std::sqrt(std::pow(m_point->x - p.x, 2) +
        std::pow(m_point->y - p.y, 2) +
        std::pow(m_point->z - p.z, 2));

    return distance;

}

void Octree::getMinOctreeDist(std::vector<Octree*> siblings, float& distance, Octree* minOct, const Point3D& pos)
{

    for (Octree* voisin : siblings)
    {
        Point3D* pv = voisin->getMpoint();
        float d = FLT_MAX;

        if (pv != nullptr)
        {
            d = voisin->getDistance(pos);

            if (d < distance)
            {
                distance = d;
                *minOct = *voisin;
            }
        }

        if (pv == nullptr && (!voisin->getChildren().empty()))
        {
            getMinOctreeDist(voisin->getChildren(), distance, minOct, pos);
        }
    }

}

void Octree::invalidate()
{
    if (m_dirty)
    {
        m_children = {};
        m_parent = nullptr;
        m_point = nullptr;

        limit_a = nullptr;
        limit_b = nullptr;
    }


}

/*
 * int main()
 *
 * {
 * Octree mytree(Point3D(),  Point3D(8.0f, 8.0, 8.0));
    mytree.insert(7.5f, 7.5f, 7.5f); // tag = 7
    std::cout << "------------------";
    mytree.insert(0.5f, 0.5f, 7.5f); // tag = 5
    std::cout << "------------------";
    mytree.insert(0.5f, 0.5f, 6.5f); // tag =
    std::cout << "------------------";
    std::cout << "------------------";
    mytree.insert(0.5f, 0.5f, 6.5f);
    mytree.find(0.5f, 0.5f, 6.5f);
    if ( ! mytree.find(4.5f, 0.5f, 6.5f))
    {
        std::cout << "Not Found" ;
        mytree.insert(4.5f, 0.5f, 6.5f);
    }
     mytree.find(4.5f, 0.5f, 6.5f);

    std::cout << "------------------";
    std::cout << "=====================================";

    Point3D a(0.0f, 0.0f ,0.0f); // 0.5f, 0.5f, 6.5f
    Point3D b(1.0f, 1.0f, 1.0f); //
    Point3D c(0.6f, 0.6f, 7.2f);
    std::cout << " Closest Point to " << a << "is " << mytree.findclosest(a);
    std::cout << " Closest Point to " << b << "is " << mytree.findclosest(b);
    std::cout << " Closest Point to " << c << "is " << *mytree.findclosest(c);

    }

*/
