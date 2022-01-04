#include "GameObject.hpp"
#include "SceneNode.h"


// static define
int GameObject::g_id = 0;

// Constructos
GameObject::GameObject(GameObject* parent, glm::vec3 center, short int textureId,
    std::string filename, std::string tag)
    : m_mesh(nullptr),
    m_ntexture(textureId),
    m_filename(filename),
    m_tag(tag), m_internal(SpaceEngine::Transform())
{
    m_position = glm::vec3(center);
    // matrice de transformation interne
    m_internal.setAsIdentity();
    m_internal.setTranslate(m_position);
    // matrice de transformation monde
    m_world = glm::translate(glm::mat4x4(1.0f), parent->Position());
    m_id = ++g_id;
};

GameObject::GameObject(SceneNode* parent, glm::vec3 center, short int textureId,
    std::string filename, std::string tag)
    : m_mesh(nullptr),
    m_ntexture(textureId),
    m_filename(filename),
    m_tag(tag), m_internal(SpaceEngine::Transform())
{

    parent->addObject(this);
    m_parent = parent;
    m_position = glm::vec3(center);
    // Change of world matrix place object in node parent
    m_world = glm::translate( glm::mat4x4(1.0f), m_parent->getPosition());
    // internal = local space from parent
    m_internal.setAsIdentity();
    m_internal.setTranslate(m_position);

    m_id = ++g_id;
};

// for prog
GameObject::GameObject(glm::vec3 center, short int textureId, std::string filename, std::string tag)
    : m_mesh(nullptr), m_parent(nullptr), 
    m_ntexture(textureId),
    m_filename(filename),
    m_tag(tag), m_internal(SpaceEngine::Transform())

{
    m_position = glm::vec3(center);
    m_world = glm::translate(glm::mat4x4(1.0f), m_position); 
    m_internal.setAsIdentity();
    m_internal.setTranslate(m_position);
    m_id = ++g_id;

};

GameObject::GameObject(float x, float y, float z)
    : m_ntexture(-1), m_mesh(nullptr),
    m_tag("Default"), m_internal(SpaceEngine::Transform())
{
    m_position = glm::vec3(x, y, z);
    m_world = glm::translate(glm::mat4x4(1.0f), m_position);
    m_internal.setAsIdentity();
    m_id = ++g_id;

};

GameObject::~GameObject()
{
    if (m_mesh != nullptr)
    {
        delete m_mesh; 
    }

    /*if (m_meshtree != nullptr)
    {
        delete m_meshtree;
    }*/

    std::cout << "Delete GameObj " << getId() << "\n";
};

// ---------------------------------------
// Apply transformation in memory
void GameObject::applyTransformation()
{

    m_transfo.applyOnPoint(m_position, m_position);

    /*for (unsigned int k = 0; k < m_nVertex; k++)
    {
        m_transfo.applyOnPoint(m_vertices[k].positions, m_position);
    }*/

}

// ==================================================
// Transformations interface at GameObject Level
// ==================================================

// Rotation angle a , angle b , angle g
// True: transfo apply on object space
// False: transfo apply in local space (node)
GameObject& GameObject::Rotate(float alpha, float beta, float gamma, bool internal)
{
    if (internal)
    {
        m_internal.addRotation(alpha, beta, gamma);
    }
    else
    {
        m_transfo.addRotation(alpha, beta, gamma);
    }
    return *this;
}


// Translation x axis, y axis, y axis
// True: transfo apply on object space
// False: transfo apply in local space (node)
GameObject& GameObject::Translate(float tx, float ty, float tz, bool internal)
{
    if (internal)
    {
        m_internal.addTranslate(tx, ty, tz);
    }
    else
    {
        m_transfo.addTranslate(tx, ty, tz);
    }
    return *this;
}

// Scaling on x axis, y axis, y axis
// True: transfo apply on object space
// False: transfo apply in local space (node)
GameObject& GameObject::Scale(float sx, float sy, float sz, bool internal)
{
    if (internal)
    {
        m_internal.addScale(sx, sy, sz);
    }
    else
    {
        m_transfo.addScale(sx, sy, sz);
    }

    return *this;
}


// ---------------------------------------
// Define Transformations
// ---------------------------------------
void GameObject::setTransformation(SpaceEngine::Transform transfo, bool internal)
{
    if (internal)
    {
        m_internal = transfo;
    }
    else
    {
        m_transfo = transfo;
    }

}


void GameObject::addTransformation(const SpaceEngine::Transform& transfo, bool internal)
{
    if (internal)
    {
        m_internal = m_internal.tfm_combine_with(transfo);
    }
    else
    {
        m_transfo = m_transfo.tfm_combine_with(transfo);
    }

}



// True: transfo apply on object space
// False: transfo apply in local space (node)
SpaceEngine::Transform GameObject::getTransformation(bool internal) const {

    if (internal)
    {
        return m_internal;
    }

    return m_transfo;

}
// -------------
// WORLD MATRIX
// -------------
// Transformations apply on world space onto
// the parents of our game object
glm::mat4x4 GameObject::getWorldMat()
{
    return  m_parent->getMatWorldTransform();
}



glm::vec3 GameObject::Position() const { return m_position; }
void GameObject::Position(float x, float y, float z)
{
    m_position = glm::vec3(x, y, z);
    m_world = glm::translate(glm::mat4x4(1.0f), m_position);
    m_internal.setTranslate(m_position[0], m_position[1], m_position[2]);
}

glm::vec4 GameObject::getWorldPosition()
{
    glm::vec3 pos = this->Position();
    SpaceEngine::Transform t_interne = this->getTransformation(true);
    SpaceEngine::Transform t_externe = this->getTransformation();
    glm::mat4x4 worldmat = this->getWorldMat();

    pos = t_interne.applyToPoint(pos, glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec4 pos4act = worldmat * glm::vec4(pos[0], pos[1], pos[2], 1.0f);
    pos4act = this->getTransformation().getMatrixTransform() * pos4act;

    return pos4act;
}

//Get a global transformation matrix to set vertices on real world
glm::mat4x4 GameObject::getTransformationAllIn()
{
    glm::mat4x4 t_interne = this->getTransformation(true).getMatrixTransform();
    glm::mat4x4 t_externe = this->getTransformation().getMatrixTransform();
    glm::mat4x4 worldmat = this->getWorldMat();

    return t_externe * worldmat * t_interne;
}


// ---------------------------------------
void GameObject::print()
{
    std::cout << "[GameObj" << m_id << "] Position " << m_position.x <<','<< m_position.y << "," << m_position.z << "\n";
}


// ---------------------------------------
// Typemesh : 
// ----------
// 0 : Cube 
// 1 : Cube
// 2 : Cube
// 3 : Plane

void GameObject::initMesh(int typemesh)
{

    if (m_filename.empty())
    {
        if (m_mesh != nullptr) { m_mesh->clear(); }
        m_mesh = new Mesh(); 

        if (m_ntexture > -1 && m_ntexture < 2)
        {
            m_mesh->initCube();
        }
        else if (m_ntexture == 3)
        {
            m_mesh->initPlane();
            std::cout << "LOG Init Grass Terrain\n";
            setTag("Terrain");
        }
        else if (typemesh == 0)
        {
            m_mesh->initCube();
            std::cout << "LOG Init Dummy Dice\n";
        }
        else if (typemesh == 3)
        {
            m_mesh->initSphere();
            std::cout << "LOG Init Sphere radius 1.0\n";
        }
        else
        {
            m_mesh->initCube();
            std::cout << "Default: Dummy Dice\n";
        }
    }
    else
    {
        // load mesh
        m_mesh = new Mesh(m_filename.c_str()); 
        if (!loadMesh(m_filename))
        {
            delete m_mesh; 
            m_mesh = new Mesh(); 
            m_mesh->initCube();
            std::cout << "Default: Dummy Dice\n";
        }

    }


    if (isTerrain())
    {
        /*glm::mat4x4 mattfm = getTransformationAllIn();
        setbbox();
        glm::vec4 avec = mattfm * glm::vec4(bbox[0], 0.0f);
        glm::vec4 bvec = mattfm * glm::vec4(bbox[1], 0.0f);

        Point3D a = Point3D(avec[0], avec[1], avec[2]);
        Point3D b = Point3D(bvec[0], bvec[1], bvec[2]);

        m_meshtree = new Octree(a, b);

        for (const VertexData& pv : m_vertices)
        {
            glm::vec4 tvec = mattfm * glm::vec4(pv.positions, 1.0f);
            Point3D c(tvec[0], tvec[1], tvec[2]);
            m_meshtree->insert(c);
        }*/
    }

};

// ---------------------------------------
// init mesh data
void GameObject::initDummyCube(float textureIndex) {

    
}




// 
// ---------------------------------------

void GameObject::initPlane(unsigned int terrainSize)
{
    /*QImage heightmap(":/heightmap-1024x1024.png");


    // afficher une surface plane (16*16 sommets) compos�e de triangles.
    unsigned int sqrtnVertex = std::sqrt(terrainSize);
    unsigned int vertexNumber = sqrtnVertex * sqrtnVertex;
    m_vertices.clear();
    m_vertices.reserve(vertexNumber);

    // defined subdivision of the plane
    float minPlane = -1.0f;
    float maxPlane = 1.0f;
    float dxy = (maxPlane - minPlane) / float(sqrtnVertex - 1);


    for (unsigned int n = 0; n < sqrtnVertex; n++)
    {
        for (unsigned int m = 0; m < sqrtnVertex; m++)
        {
            //unsigned int k = n*sqrtnVertex + m;
            float textcoordx = static_cast<float>(m) / static_cast<float>(sqrtnVertex - 1);
            float textcoordy = static_cast<float>(n) / static_cast<float>(sqrtnVertex - 1);

            int tx = static_cast<int>(textcoordx * heightmap.width());
            int ty = static_cast<int>(textcoordy * heightmap.height());
            float zheight = static_cast<float>(qGray(heightmap.pixel(tx, ty))) / 255.0f;


            m_vertices.push_back({ glm::vec3(minPlane + m * dxy, maxPlane - n * dxy,  zheight),
                                  glm::vec2(textcoordx, textcoordy), static_cast<float>(m_ntexture) });

        }
    }

    // sqrtnVertex = square root Vertex
    // Duplication to form GL_TRIANGLE STRIPS is equal to 2* (sqrtnVertex-1) - 1
    unsigned int idxcount = (sqrtnVertex - 1) * (sqrtnVertex * 2) + (2 * (sqrtnVertex - 1) - 1); //Careful update indicesNumber when creating new geometry
    unsigned int tmpCount = (sqrtnVertex - 1) * (sqrtnVertex * 2);

    m_indices.reserve(idxcount);
    std::vector<GLushort> tmpindices;
    tmpindices.reserve(tmpCount);

    for (unsigned int i = 0; i < tmpCount - 1; i += 2)
    {
        tmpindices.push_back(i / 2);
        tmpindices.push_back(i / 2 + sqrtnVertex);
    }

    std::cout << tmpCount << std::endl;
    std::cout << idxcount << std::endl;

    unsigned int countVertex = 0;  // every N=2*sqrtnVertex samples we duplicate the vertices k and k+1,
    for (unsigned int k = 0; k < tmpCount; k++)
    {
        countVertex++;
        m_indices.push_back(tmpindices[k]);

        if (countVertex % (2 * sqrtnVertex) == 0)
        {
            m_indices.push_back(tmpindices[k]);
            m_indices.push_back(tmpindices[k + 1]);
        }
    }


    // set pointers
    m_pvertices = &m_vertices[0];
    m_pindices = &m_indices[0];
    // Set size
    m_nVertex = m_vertices.size();
    this->indexCount = m_indices.size();*/
}


void GameObject::setbbox()
{
    m_mesh->setBbox(); 
    m_bbox =  m_mesh->getBbox();
    // In GameObject Repere
    m_bbox.minbbox = m_internal.getMatrixTransform() * glm::vec4(m_bbox.minbbox, 1.0); 
    m_bbox.maxbbox = m_internal.getMatrixTransform() * glm::vec4(m_bbox.minbbox, 1.0);
}



bool GameObject::loadMesh(const std::string& filename)
{
    std::vector<glm::vec3> vertices;
    std::vector<std::vector<unsigned int>> faces;


    /*bool successobj = OBJIO::open(filename, vertices, faces, true, false);
    if (!successobj)
    {
        std::cout << "Failed to open => dummy Cube";
    }
    else
    {

        m_vertices = {};
        for (unsigned int k = 0; k < vertices.size(); k++)
        {
            glm::vec2 textvec = glm::vec2(vertices[k].x(), vertices[k].y());
            m_vertices.push_back({ vertices[k], textvec, -1.0 });
        }
    }*/

    return false;

}


glm::mat4x4 GameObject::getMatTransformation()
{
    return m_transfo.getMatrixTransform();
}


bool GameObject::isCollidingWithTerrain(GameObject* other)
{
    if (!other->isTerrain())
    {
        std::cout << "Warning Cannot handle collisions with non terrain objects";
        return false;
    }

    // projection on a the box 
    /*Octree* meshtree = other->getOctree();
    Point3D* a = meshtree->getLimit(true);
    Point3D* b = meshtree->getLimit(false);
    Point3D gmoPos = Point3D(getWorldPosition()[0], getWorldPosition()[1], getWorldPosition()[2]);
    Point3D gmoPos_proj = gmoPos; */
    /*
    // Reprojection inside the meshtree
    if ( ! meshtree->insideBounds(gmoPos))
    {
        return false;
    }

    if ( ! meshtree->insideBounds(gmoPos))
    {
        if (std::abs(gmoPos.z - a->z) < std::abs(gmoPos.z - b->z))
        {
            // Az nearest
            gmoPos_proj.z = a->z + a->z * 0.01f;
        }
        else
        {
            gmoPos_proj.z = b->z - b->z * 0.01f;
        }
    }

    // find suboctant
    Octree* closest_octree = meshtree->findclosestOctree(gmoPos_proj);
    Octree op3d = Octree();
    op3d = *closest_octree;
    Point3D* closest_point = op3d.getMpoint();
    float distance = FLT_MAX;

    if (closest_point == nullptr)
    {
        closest_point =  closest_octree->findclosestPoint(gmoPos_proj, distance, &op3d);
    }

    closest_point = op3d.getMpoint();*/





    /*Octree* closest_octree = meshtree->findclosestOctree(gmoPos_proj);
    Point3D* closest_point = nullptr;

    float distance = FLT_MAX;

    if (closest_octree->getMpoint() != nullptr)
    {
        Point3D p = *closest_octree->getMpoint();
        float distance_poctree = (float) std::sqrt(std::pow(p.x - gmoPos.x, 2) +
                    std::pow(p.y - gmoPos.y, 2) +
                    std::pow(p.z - gmoPos.z, 2));


        if (distance_poctree  < distance) {
            distance = distance_poctree;
            closest_point = closest_octree->getMpoint();
        }
    }
    else // find a non empty octree next to this one
    {

    }


    // check distance with neighboorhod octant
    closest_octree->getMinOctreeDist(closest_octree->getSiblings(),
                                   distance, closest_octree, gmoPos);*/


                                   /*if (closest_point != nullptr)
                                   {
                                       Point3D dist = *closest_point - gmoPos;
                                       std::cout << "Closest Point" << *closest_point << "Dist = " << dist;
                                       // avoid deletion
                                       op3d.setDirty(true);
                                       op3d.invalidate();

                                       return dist.z > 0.0f;

                                   }*/

    //glm::vec3 point = other->getNearestPos(gmoPos.x, gmoPos.y);

    //return gmoPos.z <= point.z;

    return false;

}


glm::vec3 GameObject::getNearestPos(int x, int y)
{
    if (!isTerrain())
    {
        return glm::vec3();
    }

    glm::mat4x4 mattfm = getTransformationAllIn();
    int terrainSize = this->m_mesh->getNVertex();

    int n = std::sqrt(terrainSize);

    glm::vec4 A = mattfm * glm::vec4(m_mesh->getVertices(0).positions, 1.0f);
    glm::vec4 B = mattfm * glm::vec4(m_mesh->getVertices(terrainSize - 1).positions, 1.0f);
    float dxy = (B[0] - A[0]) / float(n);

    int nx = std::abs(x - A[0]);
    int ny = std::abs(y - A[1]);
    std::cout << "nx, ny" << nx << "," << ny;

    if (nx < 0 || ny < 0)
    {
        std::cout << "nx, ny" << nx << "," << ny;
        return glm::vec3();
    }

    int k = ny * n + nx;

    glm::vec4 pos_world = mattfm * glm::vec4(m_mesh->getVertices(k).positions, 1.0f);
    std::cout << "Position k=" << k << "/" << terrainSize - 1;
    std::cout << " Vector = " << pos_world.x << "," << pos_world.y << "," << pos_world.z;
    std::cout << std::endl; 

    return glm::vec3(pos_world[0], pos_world[1], pos_world[2]);

}
