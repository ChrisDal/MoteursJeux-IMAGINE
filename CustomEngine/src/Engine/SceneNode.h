#pragma once
#include <vector>
#include <string>
#include "GameObject.hpp"
#include "Rendering/ShaderProgram.h"


class SceneNode
{
private:
    // Tree
    SceneNode* m_parent = nullptr;
    std::vector<SceneNode*> m_children;
    std::vector<GameObject*> m_objects;

    // Space  
    glm::vec3 m_origin;
    glm::vec3 m_position;
    SpaceEngine::Transform m_tsfm_world;    // position and orientation and scaling at Time t=0
    SpaceEngine::Transform m_tsfm_internal; // Internal transformation RT

    // ID
    int m_nid = -1;
    std::string m_id;

protected:
    void setId();

public:

    static unsigned int nodeNumber;


    // Constructors 
    SceneNode();
    SceneNode(const glm::vec3& position);
    SceneNode(SceneNode* parent);
    SceneNode(SceneNode* parent, const glm::vec3& position);

    ~SceneNode();

    // -----------------------
    // HANDLING HIERARCHY 
    // -----------------------

    // Nodes  
    void addChild(SceneNode* node);
    void setParent(SceneNode* node);
    std::vector<SceneNode*> getNodes();
    SceneNode* getNode(int x);
    int getChildrenNumber() const; // child is a node

    // Game Objects of node
    void addObject(GameObject* gmo);
    std::vector<GameObject*> getObjects();
    GameObject* getObject(int x);
    int getObjectNumber() const; // Game object is an object 

    // -----------------
    // Transformations
    // ----------------

    // Internal : Transformation in object repere
    // World : Transformation in repere world
    glm::vec3 getOrigin() const { return m_origin; }
    glm::vec3 getPosition() const;
    glm::mat4x4 getMatInternalTransform();
    glm::mat4x4 getMatWorldTransform();
    SpaceEngine::Transform getInternalTransform() const;
    SpaceEngine::Transform getWorldTransform();

    // Interface
    void Rotate(float alpha, float beta, float gamma, bool internal = false);
    void Translate(float tx, float ty, float tz, bool internal = false);
    void Scale(float sx, float sy, float sz, bool internal = false);

    // Position of the sceneNode
    void setPosition(float x, float y, float z);

    // Is the root of scene graph
    bool isRoot();
    bool isValid();

    // Rendering
    // ------------
    void sceneInit(SceneNode* sNode);   //init object render
    void render(SceneNode* sNode, ShaderProgram* shader); // render objects


    std::string getId() const;
    int getIntId() const;

    void print();

    // operators
    bool operator==(const SceneNode* other);
    bool operator==(const SceneNode other);
    bool operator!=(const SceneNode* other);
    bool operator!=(const SceneNode other);

};


