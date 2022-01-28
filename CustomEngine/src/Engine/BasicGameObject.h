#pragma once

#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat3
#include <glm/mat4x4.hpp> // glm::mat4

#include <vector>
#include "Transform.h"


class SceneNode;

// Game Object empty 
class BasicGameObject
{
protected:

    int m_id = -1;
    std::string m_tag;

    glm::vec3 m_position{ 0.0, 0.0, 0.0 };

    // ------------------------
    // Scene Graph & Transforms 
    // ------------------------
    SceneNode* m_parent; // Transformation World Repere 
    SpaceEngine::Transform m_internal; // Internal Transformation 


public:

    static int g_id;

    BasicGameObject(SceneNode* parent, glm::vec3 center, std::string tag = "Default");
    BasicGameObject(); 
    // for prog
    virtual ~BasicGameObject();

    // Define Transformation
    void setTransformation(SpaceEngine::Transform transfo, bool internal = false);
    void addTransformation(const SpaceEngine::Transform& transfo, bool internal = false);
    SpaceEngine::Transform getTransformation() const; // Get Object transformation


    // Transformation in memory
    void applyTransformation();
    glm::mat4x4 getMatTransformation();
    glm::mat4x4 getWorldMat();
    virtual glm::vec4 getWorldPosition();
    glm::mat4x4 getTransformationAllIn();

    // ---------------------------------------------
    // Interface
    // ---------------------------------------------
    BasicGameObject& Rotate(float alpha, float beta, float gamma, bool internal = false);
    BasicGameObject& setRotate(float alpha, float beta, float gamma, bool internal = false);
    BasicGameObject& setRotate(const glm::vec3& rotvec, bool internal = false);

    BasicGameObject& Translate(float tx, float ty, float tz, bool internal = false);
    BasicGameObject& Translate(const glm::vec3& transvec, bool internal = false);
    BasicGameObject& setTranslate(float tx, float ty, float tz, bool internal = false);
    BasicGameObject& setTranslate(const glm::vec3& transvec, bool internal = false);

    BasicGameObject& Scale(float sx, float sy, float sz, bool internal = false);
    BasicGameObject& setScale(float sx, float sy, float sz, bool internal = false);
    BasicGameObject& setScale(const glm::vec3& scalevec, bool internal = false);

    glm::vec3 Position() const;
    void Position(float x, float y, float z);
    void Position(const glm::vec3& pos); 

    

    // Print game object informations
    virtual void print() = 0;
    virtual bool hasMesh() { return false;  }

    // ID and tag check
    int getId() const { return m_id; }
    std::string getTag() const { return m_tag; }
    void setTag(std::string _tag) { m_tag = _tag; }

    bool isMainCamera() { return m_tag.compare("MainCamera") == 0; }
    bool isTerrain() { return m_tag.compare("Terrain") == 0; }
    bool isPlayer() { return m_tag.compare("Player") == 0; }

    // Update 
    virtual void Update(float deltatime) = 0; 

    // Node 
    SceneNode* getNode() { return m_parent; }




};
