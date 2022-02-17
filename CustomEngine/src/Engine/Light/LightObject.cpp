#pragma once
#include "LightObject.h"
#include "../Sphere.h"

float LightObject::processAttenuation(float Kc, float Kl, float Kq, float distance)
{
    if (Kc < 0.0f || Kl < 0.0f || Kq < 0.0f || distance < 0.0f)
    {
        return 1.0f;
    }
 
    return 1.0f / (Kc + distance * (Kl + Kq * distance));
}

void LightObject::presetParameters(float distance)
{
    if (distance < 0.0f)
    {
        setParameters(1.0f, 1.0f, 1.0f); 
    }
    else if (distance < 7.0f)
    {
        setParameters(1.0f, 0.7f, 1.8f);
    }
    else if (distance < 13.0f)
    {
        setParameters(1.0f, 0.35f, 0.44f);
    }
    else if (distance < 20.0f)
    {
        setParameters(1.0f, 0.22f, 0.20f);
    }
    else if (distance < 32.0f)
    {
        setParameters(1.0f, 0.14f, 0.07f);
    }
    else if (distance < 50.0f)
    {
        setParameters(1.0f, 0.9f, 0.032f);
    }
    else if (distance < 65.0f)
    {
        setParameters(1.0f, 0.07f, 0.017f);
    }
    else if (distance < 100.0f)
    {
        setParameters(1.0f, 0.045f, 0.0075f);
    }
    else if (distance < 160.0f)
    {
        setParameters(1.0f, 0.027f, 0.0028f);
    }
    else if (distance < 200.f)
    {
        setParameters(1.0f, 0.022f, 0.0019f);
    }
    else if (distance < 325.0f)
    {
        setParameters(1.0f, 0.014f, 0.0007f);
    }
    else if (distance < 600.0f)
    {
        setParameters(1.0f, 0.007f, 0.0002f);
    }
    else if (distance < 3250.f)
    {
        setParameters(1.0f, 0.0014f, 0.000007f);
    }
    else
    {
        setParameters(1.0f, 0.0014f, 0.000007f);
    }
}

LightObject::LightObject(SceneNode* parent, glm::vec3 center,
    short int textureId, std::string filename,
    std::string tag)
    : BasicGameObject(parent, center, tag),
    m_mesh(nullptr),
    m_ntexture(textureId),
    m_filename(filename),
    m_type(LightType::POINT), m_distance(14.0f) // Default directional
{
    // by default init a cube 
    initMesh(textureId); 
    m_mesh->setColor(1.0f, 1.0f, 1.0f, 1.0f); 
    presetParameters(m_distance);

    m_mat = new Material(Material::m_defaults[10]); 
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

void LightObject::setMaterial(const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, const float& shininess)
{
    if (m_mat == nullptr)
    {
        m_mat = new Material(this->getColor(), amb, diff, spec, shininess);
    }
    else
    {
        m_mat->setColor(this->getColor()); // Set Mesh Color 
        m_mat->m_ambient = amb;
        m_mat->m_diffuse = diff;
        m_mat->m_specular = spec;
        m_mat->setShininess(shininess);
    }
}

void LightObject::setType(LightType type, float distance)
{
    m_type = type;
    m_distance = distance;

    if (m_type == LightType::POINT)
    {
        // predefined data 
        presetParameters(m_distance);
    }

    m_Fatt = processAttenuation(m_kc, m_kl, m_kq, m_distance);
}

void LightObject::setType(unsigned int type, float distance)
{
    m_type = static_cast<LightType>(type); 

    m_distance = distance; 
    if (m_type == LightType::POINT) 
    {
        // predefined data 
        presetParameters(m_distance);
    }

    m_Fatt = processAttenuation(m_kc, m_kl, m_kq, m_distance);
}

std::string LightObject::getStringType() const
{
    std::string typeString = ""; 
    switch (m_type)
    {
    case(LightType::DIRECTIONAL): typeString = "DIRECTIONAL"; break; 
    case(LightType::POINT):       typeString = "POINT"; break; 
    case(LightType::SPOT):        typeString = "SPOT"; break; 
    case(LightType::FLASH):       typeString = "FLASH"; break; 
    default: 
        typeString = "EMPTY"; 
        break; 
    }

    return typeString; 
}

void LightObject::setParameters(float kc, float kl, float kq)
{
    m_kc = kc; 
    m_kl = kl; 
    m_kq = kq; 
}

void LightObject::Update(float deltatime)
{
    // pass 
    /*m_position.z += std::sin(0.2f) * deltatime;
    m_position.x += std::cos(0.2f) * deltatime;
    this->Rotate(0.5f, 0.0f, 0.0f, true); */
    
    
}