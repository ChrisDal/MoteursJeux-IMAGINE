#include "SceneNode.h"

// static define
unsigned int SceneNode::nodeNumber = 0;


// ===================
// CONSTRUCTORS
// ===================


SceneNode::SceneNode()
    : m_origin(glm::vec3(0.0f, .0f, 0.0f)),
    m_position(glm::vec3(0.0f, .0f, 0.0f)),
    m_tsfm_world(SpaceEngine::Transform())
{
    m_nid = ++nodeNumber;
    m_tsfm_world.setTranslate(m_origin);
    setId();
}

SceneNode::SceneNode(const glm::vec3& position)
    : m_origin(position), m_tsfm_world(SpaceEngine::Transform()), m_position(position)
{
    m_nid = ++nodeNumber;
    m_tsfm_world.setTranslate(m_origin);
    setId();
}



SceneNode::SceneNode(SceneNode* parent)
    : m_tsfm_world(SpaceEngine::Transform()),
    m_position(glm::vec3())
{
    parent->addChild(this);
    setParent(parent);
    m_origin = parent->getOrigin();
    m_tsfm_world.setTranslate(m_origin);
    m_nid = ++nodeNumber;
    setId();
}

SceneNode::SceneNode(SceneNode* parent, const glm::vec3& position)
    : m_tsfm_world(SpaceEngine::Transform()),
    m_position(position)
{
    parent->addChild(this);
    setParent(parent);
    m_origin = parent->getOrigin();
    m_position = parent->getOrigin() + position;
    m_tsfm_world.setTranslate(parent->getOrigin());
    m_tsfm_world.setTranslate(position);

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
    if (m_object != nullptr)
    {
        delete m_object; 
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
    return static_cast<int>(m_children.size());
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
        getObject()->Rotate(alpha, beta, gamma, true);
    }
    else
    {
        m_tsfm_world.addRotation(alpha, beta, gamma);
    }
}

void SceneNode::Translate(float tx, float ty, float tz, bool internal) {

    if (internal)
    {
        getObject()->Translate(tx, ty, tz, true);
    }
    else
    {
        m_tsfm_world.addTranslate(tx, ty, tz);
    }
}


void SceneNode::Translate(const glm::vec3& txyz, bool internal) {

    if (internal)
    {
        getObject()->Translate(txyz, true);
    }
    else
    {
        m_tsfm_world.addTranslate(txyz);
    }
}

void SceneNode::Scale(float sx, float sy, float sz, bool internal) {

    if (internal)
    {
        getObject()->Scale(sx, sy, sz);
    }
    else
    {
        m_tsfm_world.addScale(sx, sy, sz);
    }
}
// Set Matrix Transformation if internal set GameObject transformation matrix 
// Else set Node Matrix Transformation 
void SceneNode::setTransformation(const SpaceEngine::Transform& transformation, bool internal)
{
    if (!internal) {
        m_tsfm_world = transformation;
    }
    else
    {
        if (m_object != nullptr) {
            m_object->setTransformation(transformation, true); 
        }
    }
}

void SceneNode::addTransformation(const SpaceEngine::Transform& transformation, bool internal)
{
    SpaceEngine::Transform transfo(m_tsfm_world);

    transfo.tfm_combine_with(transformation); 
    m_tsfm_world = transfo; 
    
}

void SceneNode::setPosition(float x, float y, float z)
{
    glm::vec3 translation = m_position - glm::vec3(x, y, z); 
    Translate(translation.x, translation.y, translation.z);
    m_position = glm::vec3(x, y, z); 
}


glm::vec3 SceneNode::getPosition() const
{
    return m_position;
}



// ===================
// GAME OBJECTS 
// ===================
void SceneNode::addObject(BasicGameObject* gmo)
{
    // Node is responsible for gameobject deletion 
    if (m_object != nullptr) {
        delete m_object; 
    }

    m_object = gmo; 
}


BasicGameObject* SceneNode::getObject()
{
    if (m_object == nullptr)
    {
        std::cout << "Empty Child for Node " << getId() << std::endl;
    }
    return m_object;
}

/// <summary>
/// Search and return GameObject by Id(int)
/// </summary>
/// <param name="idobj"></param>
/// <returns>GameObject* </returns>
BasicGameObject* SceneNode::getObjectbyID( int& idobj)
{

    if (this->haveGmo()) 
    {
        if (getObject()->getId() == idobj)
        {
            return this->getObject();
        }
    } 
    
    if (m_children.size() > 0)
    {
        for (SceneNode* child : m_children) {
            if (child->getObjectbyID(idobj) != nullptr)
            {
                return child->getObjectbyID(idobj); 
            }
        }
    }

    return nullptr;


}






// =========================================
// DIVERS
// --------------------------



void SceneNode::print()
{
    std::cout << " Node Id : " << getId().c_str() << ", Node number : " << m_nid << " ";
    
    if (haveGmo()) {
        m_object->print();
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


    // invalid object
    if (sNode->getObject() == nullptr) {
        std::cout << "Invalid GameObject, Parent Node:" << sNode->getId().c_str();
        return; 
    }

    std::cout << "  init Rendering Obj=" << sNode->getObject()->getId() << "\n";



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




void SceneNode::Update(float deltatime)
{
    
    if (m_object != nullptr) {
        m_object->Update(deltatime);
    }
    

    for (int i = 0; i < getChildrenNumber(); i++)
    {
        getNode(i)->Update(deltatime);
    }
}


// ========================
//   NODE TRANSFORMATION
// ========================
// Each node contains a transformation 
// If a node contains a game Object then the total transformation is :
// T = Transformation(Node) * Transformation(GameObject)
// Thus any child of the node will be the child of the GameObject associated
// T(node) can be seen as the external transformation 
// T(Game Object) as the local transformation 

glm::mat4x4 SceneNode::getMatWorldTransform() {

    glm::mat4x4 transfoMat = m_tsfm_world.getMatrixTransform();

    if (m_parent != nullptr)
    {
        // Add object transformation
        if (m_parent->haveGmo()) {
            transfoMat = transfoMat * m_parent->getObject()->getMatTransformation();
            return m_parent->getMatWorldTransform() * transfoMat;
        }

        return m_parent->getMatWorldTransform() * transfoMat;
    }

    return transfoMat;

}



// Get One node Transformation
SpaceEngine::Transform SceneNode::getNodeTransform()
{
    SpaceEngine::Transform objtransfo = SpaceEngine::Transform(m_tsfm_world); 
    if (m_object != nullptr) 
    {
        objtransfo.tfm_combine_with(m_object->getTransformation()); 
    }

    return objtransfo;
}

glm::mat4x4 SceneNode::getMatNodeTransform()
{
    glm::mat4x4 objtransfo = m_object != nullptr ? m_object->getMatTransformation() : glm::mat4x4(1.0f); 
    
    return m_tsfm_world.getMatrixTransform() * objtransfo;
}



// Get Matrix of Total World Transformation
glm::mat4x4 SceneNode::getMatTotalNodeTransform()
{
    glm::mat4x4 transfo = m_object->getMatTransformation(); 
    return this->getMatWorldTransform() * transfo;
}


/// <summary>
/// Return Recursive transformation
/// </summary>
/// <returns></returns>
glm::mat4x4 SceneNode::getMatTransform()
{
    if (m_parent != nullptr) {
        return m_parent->getMatNodeTransform() * m_tsfm_world.getMatrixTransform();
    }
    
    return m_tsfm_world.getMatrixTransform();
    
}


// ==============
//   OPERATORS
// ==============

/// <summary>
/// Return Node if found from string ID "SNX" 
/// With a max depth research allowed 
/// </summary>
/// <param name="sId"></param>
/// <param name="maxDepth">-1 no maxDepth</param>
/// <returns></returns>
SceneNode* SceneNode::getNodebyId(const std::string& sId, const int& maxDepth)
{
    // Look into children 
    if (m_children.size() == 0) {
        return nullptr;
    }

    for (SceneNode* child : m_children) {
        
        if ( ! child->getId().compare(sId)) {
            // Found 
            return child; 
        }

        if (maxDepth == 0) {
            // dont call for recursivity - last depth 
            continue;
        }

        // Go down 
        SceneNode* idNode = child->getNodebyId(sId);
        if (idNode != nullptr)
        {
            return idNode; 
        }
    }
    
    return nullptr; 
}


SceneNode* SceneNode::getNodebyId(const int& sId, const int& maxDepth)
{
    
    // Look into children 
    if (m_children.size() == 0) {
        return nullptr;
    }

    SceneNode* foundId = nullptr;

    for (SceneNode* child : m_children) {

        if ( ! (child->getIntId()  == sId)) {
            // Found 
            foundId = child;
            return foundId;
        }

        if (maxDepth == 0) {
            // dont call for recursivity - last depth 
            continue; 
        }

        // Go down - if found return else keep going 
        foundId = child->getNodebyId(sId, maxDepth - 1 );
        if (foundId != nullptr)
        {
            return foundId;
        }
    }

    return foundId; 
}


