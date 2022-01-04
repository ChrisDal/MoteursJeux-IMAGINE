#include "SceneNode.h"

// static define
unsigned int SceneNode::nodeNumber = 0;


// ===================
// CONSTRUCTORS
// ===================


SceneNode::SceneNode()
    : m_origin(glm::vec3(0.0f, .0f, 0.0f)),
    m_position(glm::vec3(0.0f, .0f, 0.0f)),
    m_tsfm_world(SpaceEngine::Transform()),
    m_tsfm_internal(SpaceEngine::Transform())
{
    m_nid = ++nodeNumber;
    m_tsfm_internal.setAsIdentity();
    m_tsfm_world.setTranslate(m_origin);
    setId();
}

SceneNode::SceneNode(const glm::vec3& position)
    : m_origin(position), m_tsfm_world(SpaceEngine::Transform()),
    m_tsfm_internal(SpaceEngine::Transform()), m_position(position)
{
    m_nid = ++nodeNumber;
    m_tsfm_world.setTranslate(m_origin);
    m_tsfm_internal.setAsIdentity();
    setId();
}



SceneNode::SceneNode(SceneNode* parent)
    : m_tsfm_world(SpaceEngine::Transform()),
    m_tsfm_internal(SpaceEngine::Transform()),
    m_position(glm::vec3())
{
    parent->addChild(this);
    setParent(parent);
    m_origin = parent->getOrigin();
    m_tsfm_world.setTranslate(m_origin);
    m_tsfm_internal.setAsIdentity();
    m_nid = ++nodeNumber;
    setId();
}

SceneNode::SceneNode(SceneNode* parent, const glm::vec3& position)
    : m_tsfm_world(SpaceEngine::Transform()),
    m_tsfm_internal(SpaceEngine::Transform()),
    m_position(position)
{
    parent->addChild(this);
    setParent(parent);
    m_origin = parent->getOrigin();
    m_position = parent->getOrigin() + position;
    m_tsfm_world.setTranslate(parent->getOrigin());
    m_tsfm_world.setTranslate(position);

    m_tsfm_internal.setAsIdentity();
    m_nid = ++nodeNumber;
    setId();
}




// ===================
// DESTRUCTORS
// ===================

SceneNode::~SceneNode()
{
    std::cout << "Destructor Node" << m_id << " \n";
    m_nid = -1; // invalidate Id (in Int) 

    // Remove Game Object at this node
    for (unsigned int k = 0; k < m_objects.size(); k++)
    {
        delete m_objects.at(k);
    }

    // Remove children (other scene node)
    if (m_children.size() > 0)
    {
        for (unsigned int k = 0; k < m_children.size(); k++)
        {
            delete m_children.at(k);
        }
    }

    // invalidate Id when emptied (comparison to invalid is possible)
    setId();
}


// =====================
// 		   NODES
// =====================


// add a child
void SceneNode::addChild(SceneNode* node)
{
    if (node == this) { return; }

    m_children.push_back(node);
    node->setParent(this);
}

void SceneNode::setParent(SceneNode* node)
{
    this->m_parent = node;
}

// no parent == root
bool SceneNode::isRoot()
{
    return m_parent == nullptr;
}

bool SceneNode::isValid()
{
    return (m_nid > -1) && (m_id.compare("invalid") && (!m_id.empty()));
}


int SceneNode::getChildrenNumber() const
{
    return m_children.size();
}

std::vector<SceneNode*> SceneNode::getNodes()
{
    return m_children;
}
SceneNode* SceneNode::getNode(int x)
{
    return m_children.at(x);
}

// Get Id
std::string SceneNode::getId() const { return m_id; }

int SceneNode::getIntId() const
{
    return m_nid;
}


// Set Id at the creation
void SceneNode::setId()
{
    if (m_nid > -1)
    {
        m_id = "SN" + std::to_string(m_nid);
    }
    else
    {
        m_id = "invalid";
    }

}


// ==========================
// TRANSFORMATIONS INTERFACE
// ==========================
void SceneNode::Rotate(float alpha, float beta, float gamma, bool internal)
{
    if (internal)
    {
        m_tsfm_internal.addRotation(alpha, beta, gamma);
    }
    else
    {
        m_tsfm_world.addRotation(alpha, beta, gamma);
    }
}

void SceneNode::Translate(float tx, float ty, float tz, bool internal) {

    if (internal)
    {
        m_tsfm_internal.addTranslate(tx, ty, tz);
    }
    else
    {
        m_tsfm_world.addTranslate(tx, ty, tz);
    }
}

void SceneNode::Scale(float sx, float sy, float sz, bool internal) {

    if (internal)
    {
        m_tsfm_internal.addScale(sx, sy, sz);
    }
    else
    {
        m_tsfm_world.addScale(sx, sy, sz);
    }
}


glm::vec3 SceneNode::getPosition() const
{
    return m_position;
}



// ===================
// GAME OBJECTS 
// ===================
void SceneNode::addObject(GameObject* gmo)
{
    m_objects.push_back(gmo);
}


std::vector<GameObject*> SceneNode::getObjects()
{
    return m_objects;
}

GameObject* SceneNode::getObject(int x)
{
    return m_objects.at(x);
}

int SceneNode::getObjectNumber() const
{
    return m_objects.size();
}



// =========================================
// DIVERS
// --------------------------



void SceneNode::print()
{
    std::cout << " Node Id : " << getId().c_str() << ", Node number : " << m_nid << " ";

    for (unsigned int x = 0; x < m_objects.size(); x++)
    {
        m_objects[x]->print();
    }

    for (unsigned int x = 0; x < m_children.size(); x++)
    {
        std::cout << " | ";
        m_children[x]->print();
    }

}


// =========================================
// INIT RENDERING 
// =========================================

// TODO : if fail init rendering remove object from scene graph data
void SceneNode::sceneInit(SceneNode* sNode)
{

    if (sNode == nullptr || (!sNode->getId().compare("invalid")))
    {
        std::cout << "Invalid Node , parent = " << m_parent->getId().c_str() << "\n";
        return;
    }

    std::cout << "Node " << sNode->getIntId();

    for (int k = 0; k < sNode->getObjectNumber(); k++)
    {
        // invalid object
        if (sNode->getObject(k) == nullptr) {
            std::cout << "Invalid GameObject, Parent Node:" << sNode->getId().c_str();
            break;
        }

        // k is the type of texture by default 0 or not
        //sNode->getObject(k)->initMesh(k);
        std::cout << "  init Rendering Obj=" << sNode->getObject(k)->getId() << "\n";
    }


    for (int i = 0; i < sNode->getChildrenNumber(); i++)
    {
        sceneInit(sNode->getNode(i));
        std::cout << "  init children Node= " << i << "\n";
    }

    return;
}

// Scene Rendering : call render on each game object
void SceneNode::render(SceneNode* sNode, ShaderProgram* shader)
{
    // init each game object for rendering at
    // hierarchy node
    /*for (int k = 0; k < sNode->getObjectNumber(); k++)
    {
        sNode->getObject(k)->Draw(shader);
    }

    for (int i = 0; i < sNode->getChildrenNumber(); i++)
    {
        render(sNode->getNode(i), shader);
    }*/

    return;
}


// ========================
//   NODE TRANSFORMATION
// ========================

glm::mat4x4 SceneNode::getMatInternalTransform() {
    return m_tsfm_internal.getMatrixTransform();
}
glm::mat4x4 SceneNode::getMatWorldTransform() {

    if (m_parent != nullptr)
    {
        return m_parent->getMatWorldTransform() * m_tsfm_world.getMatrixTransform();
    }

    return m_tsfm_world.getMatrixTransform();

}
SpaceEngine::Transform SceneNode::getInternalTransform() const {
    return m_tsfm_internal;
}
SpaceEngine::Transform SceneNode::getWorldTransform() {

    SpaceEngine::Transform transfo(m_tsfm_world);

    if (m_parent != nullptr)
    {
        transfo.tfm_combine_with(m_parent->getWorldTransform());
    }


    return transfo;
}




// ==============
//   OPERATORS
// ==============

bool SceneNode::operator==(const SceneNode* other)
{
    return this->getId() == other->getId();
}

bool SceneNode::operator!=(const SceneNode* other)
{
    return !(this == other);
}

bool SceneNode::operator==(const SceneNode other)
{
    return getId() == other.getId();
}

bool SceneNode::operator!=(const SceneNode other)
{
    return !(*this == other);
}
