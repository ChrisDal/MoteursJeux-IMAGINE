#pragma once

#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4


#include "BasicGameObject.h"
#include "Mesh.h"



class SceneNode;

class LightObject : public BasicGameObject
{
private:


    // Mesh
    Mesh* m_mesh;
    Material* m_mat;

    short int m_ntexture;

    //obj file
    std::string m_filename;

public:
    //static int g_id;

    LightObject(SceneNode* parent, glm::vec3 center,
        short int textureId = -1, std::string filename = "",
        std::string tag = "Light");
    ~LightObject();


    // Print game object informations
    void print() override;
    bool hasMesh() override { return true; }
    bool hasMaterial() const { return m_mat != nullptr; }
    virtual bool isLight() override { return true; }

    // Mesh
    void initMesh(int typemesh);
    void initMesh(const char* filename);
    bool loadMesh(const std::string& filename);
    void setColor(const glm::vec4& color) { m_mesh->setColor(color); }

    // Material  
    void initMaterial(Texture* texture, const glm::vec3& color);

    // Rendering
    Mesh* getMesh() const { return m_mesh; }
    Material* getMaterial() const { return m_mat; }


    // Update 
    void Update(float deltatime) override;


};
