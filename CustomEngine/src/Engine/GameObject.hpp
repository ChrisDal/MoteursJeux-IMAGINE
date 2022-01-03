#pragma once

#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4

#include <vector>
#include "Transform.h"
#include "Mesh.h"
#include "Rendering/VertexData.h"
#include "Rendering/ShaderProgram.h"
//#include "Octree.hpp"

class SceneNode;

class GameObject
{
private:

    int m_id = -1;

    glm::vec3 m_position = glm::vec3(0.0, 0.0, 0.0);

    // Hierarchie
    std::vector<GameObject*> m_children;
    SceneNode* m_parent;

    // Mesh
    Mesh* m_mesh; 
    SpaceEngine::boundingBox m_bbox; 

    // -----------

    short int m_ntexture;

    //obj file
    std::string m_filename;
    std::string m_tag;

    // Octree Data structure
    //Octree* m_meshtree = nullptr;


    // -----------------
    // Transformations
    // -------------------
    SpaceEngine::Transform m_transfo;   // transformation en temps réel repere world
    SpaceEngine::Transform m_internal;  // transformation repere interne
    glm::mat4x4 m_world;                // transformation at time T=0 , position in world


protected:

    // init mesh data
    void initDummyCube(float textureindex = 1.0f);
    void initPlane(unsigned int terrainSize);
    // BoundingBox 
    void setbbox();

public:
    static int g_id;

    GameObject(SceneNode* parent, glm::vec3 center,
        short int textureId = -1, std::string filename = "",
        std::string tag = "Default");
    GameObject(GameObject* parent, glm::vec3 center,
        short int textureId = -1, std::string filename = "",
        std::string tag = "Default");
    // for prog
    GameObject(glm::vec3 center, short int textureId = -1, std::string filename = "", std::string tag = "Default");
    GameObject(float x, float y, float z);
    ~GameObject();

    // Define Transformation
    void setTransformation(SpaceEngine::Transform transfo, bool internal = false);
    void addTransformation(const SpaceEngine::Transform& transfo, bool internal = false);
    SpaceEngine::Transform getTransformation(bool internal = false) const;


    // Transformation in memory
    void applyTransformation();
    glm::mat4x4 getMatTransformation();
    glm::mat4x4 getWorldMat();
    glm::vec4 getWorldPosition();
    glm::mat4x4 getTransformationAllIn();

    // Interface
    GameObject& Rotate(float alpha, float beta, float gamma, bool internal = false);
    GameObject& Translate(float tx, float ty, float tz, bool internal = false);
    GameObject& Scale(float sx, float sy, float sz, bool internal = false);
    glm::vec3 Position() const;
    void Position(float x, float y, float z);

    // Print game object informations
    void print();

    // Mesh
    void initMesh(int typemesh);
    void initMesh(const char* filename);
    bool loadMesh(const std::string& filename);

    // Rendering
    Mesh* getMesh() const { return m_mesh; }
  
    // Collisions 
    bool isCollidingWithTerrain(GameObject* other);

    // ID and tag check
    int getId() const { return m_id; }
    std::string getTag() const { return m_tag; }
    void setTag(std::string _tag) { m_tag = _tag; }
    bool isMainCamera() { return m_tag.compare("MainCamera") == 0; }
    bool isTerrain()    { return m_tag.compare("Terrain") == 0; }
    bool isPlayer()     { return m_tag.compare("Player") == 0; }
    
    //Octree* getOctree() { return m_meshtree; }

    glm::vec3 getNearestPos(int x, int y);

};

