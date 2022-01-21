#pragma once
#include <vector>
#include <string>

#include "BasicGameObject.h"
#include "Rendering/ShaderProgram.h"


class SceneNode
{
private:
    // Tree
    SceneNode* m_parent = nullptr;
    std::vector<SceneNode*> m_children;
    BasicGameObject* m_object; // One gameObject per node 
    //std::vector<BasicGameObject*> m_objects;

    // Space  
    glm::vec3 m_origin;
    glm::vec3 m_position;
    // Transformation
    SpaceEngine::Transform m_tsfm_world;    // position and orientation and scaling at Time t=0
    //SpaceEngine::Transform m_tsfm_internal; // Internal transformation RT

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

    // Nodes  : multiple children
    void addChild(SceneNode* node);
    void setParent(SceneNode* node);
    std::vector<SceneNode*> getNodes();
    SceneNode* getNode(int x);
    int getChildrenNumber() const; 

    // One GameObject Per node 
    void addObject(BasicGameObject* gmo);
    BasicGameObject* getObject();

    // -----------------
    // Transformations
    // ----------------

    // Internal : Transformation in object repere
    // World : Transformation in repere world
    glm::vec3 getOrigin() const { return m_origin; }
    glm::vec3 getPosition() const;
    // Get Transform to World 
    glm::mat4x4 getMatWorldTransform();
    SpaceEngine::Transform getWorldTransform();
    // Get Node Matrix 
    SpaceEngine::Transform getNodeTransform(); // Get Node + GameObject transform
    glm::mat4x4 getMatNodeTransform(); // Get Node + GameObject transform
    // Get All in One : SceneNode transform + GameObject 
    SpaceEngine::Transform getTotalNodeTransform(); // Get Node + GameObject transform
    glm::mat4x4 getMatTotalNodeTransform(); // Get Node + GameObject transform
    glm::mat4x4 getMatTransform();
    // Get Transformation of the Node 
    SpaceEngine::Transform getTransform() const { return m_tsfm_world; }
    glm::mat4x4 getMatTransform() const { return getTransform().getMatrixTransform(); }

    // Interface
    void Rotate(float alpha, float beta, float gamma, bool internal = false);
    void Translate(float tx, float ty, float tz, bool internal = false);
    void Translate(const glm::vec3& txyz, bool internal= false);
    void Scale(float sx, float sy, float sz, bool internal = false);
    void setTransformation(const SpaceEngine::Transform& transformation, bool internal=false); 
    void addTransformation(const SpaceEngine::Transform& transformation, bool internal = false);

    // Position of the sceneNode
    void setPosition(float x, float y, float z);

    // Is the root of scene graph
    bool isRoot();
    bool isValid();

    // Rendering
    // ------------
    void sceneInit(SceneNode* sNode);   //init object render
    void render(SceneNode* sNode, ShaderProgram* shader); // render objects

    // Update 
    void Update(float deltatime); 


    std::string getId() const;
    int getIntId() const;

    void print();

    // have a valid game object 
    inline bool haveGmo() const { return m_object != nullptr;  }
    SceneNode* getNodebyId(const std::string& sId, const int& maxDepth = -1);                 //!< From this node look for node by string ID (down)
    SceneNode* getNodebyId(const int& sId, const int& maxDepth=-1);                         //!< From this node look for node by int ID (down)

    SceneNode* getParent() { return m_parent;  }

    // operators
    bool operator==(const SceneNode* other);
    bool operator==(const SceneNode other);
    bool operator!=(const SceneNode* other);
    bool operator!=(const SceneNode other);

};


