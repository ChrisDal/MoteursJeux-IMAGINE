#pragma once

#include <vector>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4


#include "geometryengine.h"
#include "Transform.h"

#include "Vdata.h"
#include "Octree.hpp"

class SceneNode;

class BasicGameObject
{
private:

    int m_id = -1;

    glm::vec3 m_position = glm::vec3(0.0, 0.0, 0.0);

    // Hierarchie
    std::vector<BasicGameObject*> m_children;
    SceneNode* m_parent;


    // data to draw - Cube geometry
    std::vector<VertexData> m_vertices;
    std::vector<glm::vec3> bbox;
    std::vector<GLushort> m_indices;
    unsigned int m_nVertex;
    unsigned int indexCount;

    GeometryEngine* m_geom;
    VertexData* m_pvertices;
    GLushort* m_pindices;

    short int m_ntexture;

    //obj file
    std::string m_filename;
    std::string m_tag;

    // Octree Data structure
    Octree* m_meshtree = nullptr;


    // ----------------
    // Transformations 
    // ----------------
    Transform m_transfo;  // transformation en temps réel repere world
    Transform m_internal; // transformation repere interne
    glm::mat4x4 m_world;   // transformation at time T=0 , position in world


protected:

    // OpenGL Render data
    unsigned int getNVertex() const { return  m_nVertex; }
    unsigned int getIndexCount() const { return indexCount; }
    VertexData* getVertices() { return m_vertices.data(); }
    GLushort* getIndices() { return  m_indices.data(); }

    // init mesh data
    void initDummyCube(float textureindex = 1.0f);
    void initPlane(unsigned int terrainSize);

    void setbbox();

public:
    static int g_id;

    BasicGameObject(SceneNode* parent, glm::vec3 center,
        short int textureId = -1, std::string filename = "",
        std::string tag = "Default");
    BasicGameObject(BasicGameObject* parent, glm::vec3 center,
        short int textureId = -1, std::string filename = "",
        std::string tag = "Default");
    // for prog
    ~BasicGameObject();

    // Define Transformation
    void setTransformation(Transform transfo, bool internal = false);
    void addTransformation(const Transform& transfo, bool internal = false);
    Transform getTransformation(bool internal = false) const;


    // Transformation in memory
    void applyTransformation();
    glm::mat4x4 getMatTransformation();
    glm::mat4x4 getWorldMat();
    glm::vec4 getWorldPosition();
    glm::mat4x4 getTransformationAllIn();

    // Interface
    BasicGameObject& Rotate(float alpha, float beta, float gamma,
        bool internal = false);
    BasicGameObject& Translate(float tx, float ty, float tz,
        bool internal = false);
    BasicGameObject& Scale(float sx, float sy, float sz,
        bool internal = false);
    glm::vec3 Position() const;
    void Position(float x, float y, float z);


    // Print game object informations
    void print();

    // Mesh
    virtual void initMesh(int typemesh);
    virtual void initMesh(const char* filename);
    virtual bool loadMesh(const std::string& filename);

    // Rendering
    virtual void initRendering();
    virtual void render(QOpenGLShaderProgram* program);
    GeometryEngine* getGeomEng() const;



    // ID and tag check
    int getId() const { return m_id; }
    std::string getTag() const { return m_tag; }
    void setTag(std::string _tag) { m_tag = _tag; }
    // Get type 
    bool isMainCamera() { return m_tag.compare("MainCamera") == 0; }
    bool isTerrain() { return m_tag.compare("Terrain") == 0; }
    bool isPlayer() { return m_tag.compare("Player") == 0; }




};
