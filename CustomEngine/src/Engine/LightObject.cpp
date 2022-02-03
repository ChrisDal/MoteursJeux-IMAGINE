#pragma once
#include "LightObject.h"
#include "Sphere.h"

LightObject::LightObject(SceneNode* parent, glm::vec3 center, 
                        short int textureId,  std::string filename,
                        std::string tag)
    : BasicGameObject(parent, center, tag),
    m_mesh(nullptr),
    m_ntexture(textureId),
    m_filename(filename), m_mat(nullptr)
{
    // by default init a cube 
    initMesh(textureId); 
    m_mesh->setColor(1.0f, 1.0f, 1.0f, 1.0f); 

}


LightObject::~LightObject()
{
    if (m_mesh != nullptr)
    {
        delete m_mesh;
    }

    if (m_mat != nullptr)
    {
        delete m_mat;
    }

    std::cout << "Delete Light " << getId() << "\n";
};


// ---------------------------------------
void LightObject::print()
{
    std::cout << "[LightObj" << m_id << "] Position " 
                << m_position.x << ',' << m_position.y << "," << m_position.z << "\n";
}

// ---------------------------------------
// Typemesh : 
// ----------
// 0 : Cube 
// 1 : Quad
// 2 : Capsule
// 3 : Sphere

void LightObject::initMesh(int typemesh)
{
    bool noforce = true; // do not force mesh // prog 

    if (m_filename.empty() && noforce)
    {
        if (m_mesh != nullptr) { 
            m_mesh->clear(); 
        }
        else
        {
            m_mesh = new Mesh();
        }

        
        std::cout << "[" <<  getTag() << " " << getId() << "] "; 

        switch (typemesh)
        {
        case 0: 
            m_mesh->initCube(); 
            std::cout << "init Cube\n"; 
            break; 
        case 1: 
            m_mesh->initQuad();
            std::cout << "init Quad\n";
            break;
        case 2: 
            m_mesh->initCapsule(0.25f, 0.5f);
            std::cout << "init Capsule\n";
            break;
        case 3: 
            delete m_mesh; 
            m_mesh = new Sphere(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            std::cout << "init Sphere\n";
            break; 
        default: 
            // Cube 
            m_mesh->initCube();
            std::cout << "init Default LightCube\n";
            break;
        }; 

    }
    else
    {
        // load mesh
        initMesh(m_filename.c_str()); 
    }


}
void LightObject::initMesh(const char* filename)
{
    if (m_mesh != nullptr) 
    {
        m_mesh->clear();
    }
    else 
    {
        m_mesh = new Mesh(filename);
    }

}


void LightObject::initMaterial(Texture* texture, const glm::vec3& color)
{
    m_mat = new Material(texture, color);
}

void LightObject::Update(float deltatime)
{
    // pass 
    m_position.z += std::sin(0.2f) * deltatime;
    m_position.x += std::cos(0.2f) * deltatime;
    this->Rotate(0.5f, 0.0f, 0.0f, true); 
    
    
}