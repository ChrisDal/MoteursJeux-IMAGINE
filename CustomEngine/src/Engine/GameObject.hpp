#pragma once

#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4

#include <vector>
#include "BasicGameObject.h"
#include "Mesh.h"
#include "Components.h"
#include "Rendering/VertexData.h"
#include "Rendering/ShaderProgram.h"


class SceneNode;

class GameObject : public BasicGameObject
{
private:


    // Mesh
    Mesh* m_mesh; 
    Material* m_mat; 
    SpaceEngine::boundingBox m_bbox; 

    // -----------

    short int m_ntexture;

    //obj file
    std::string m_filename;

    // Octree Data structure
    //Octree* m_meshtree = nullptr;

protected:

    // init mesh data
    void initDummyCube(float textureindex = 1.0f);
    void initPlane(unsigned int terrainSize);
    // BoundingBox 
    void setbbox();

public : // Components 

    void addComponent(Component* component); 
    Velocity velocity;
    Living alive; 

public:
    //static int g_id;

    GameObject(SceneNode* parent, glm::vec3 center,
        short int textureId = -1, std::string filename = "",
        std::string tag = "Default");
    /*GameObject(GameObject* parent, glm::vec3 center,
        short int textureId = -1, std::string filename = "",
        std::string tag = "Default");*/
    // for prog
    GameObject(glm::vec3 center, short int textureId = -1, std::string filename = "", std::string tag = "Default");
    GameObject(float x, float y, float z);
    ~GameObject();


    // Print game object informations
    void print() override;
    virtual bool hasMesh() override { return true; }
    virtual bool hasMaterial() const { return m_mat != nullptr;  }
    virtual bool isLight() override { return false; }

    // Mesh
    void initMesh(int typemesh);
    void initMesh(const char* filename);
    bool loadMesh(const std::string& filename);

    // Material  
    void initMaterial(Texture* texture, const glm::vec3& color);
    

    // Rendering
    Mesh* getMesh() const { return m_mesh; }
    Material* getMaterial() const { return m_mat; }


    // Game 
    


    // Update 
    void Update(float deltatime) override; 

  
    // Collisions 
    bool isCollidingWithTerrain(GameObject* other);

    //Octree* getOctree() { return m_meshtree; }

    glm::vec3 getNearestPos(int x, int y);

    // Operators
    inline bool operator==(const GameObject* other) {
        return this->getId() == other->getId();
    }

    inline bool operator!=(const GameObject* other) {
        return !(this == other);
    }

    inline bool operator==(const GameObject other) {
        return getId() == other.getId();
    }

    inline bool operator!=(const GameObject other) {
        return !(*this == other);
    }

};

