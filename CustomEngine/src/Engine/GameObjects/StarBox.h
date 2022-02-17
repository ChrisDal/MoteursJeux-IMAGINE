#pragma once

#include <iostream>
#include "../BasicGameObject.h"
#include "../Mesh.h"

class SceneNode;

class StarBox : public BasicGameObject
{

protected:

    // Mesh
    Mesh* m_mesh;
    int m_nstars; 
    float m_radiusStars; 


protected:
    // BoundingBox 
    void setbbox();

public:

    StarBox(SceneNode* parent, glm::vec3 center,
        std::string tag = "StarBox", 
        float radius=2.0f, int stars = 100);
    ~StarBox(); 


    // Print game object informations
    void print() override;
    
    virtual bool hasMesh() override { return m_mesh != nullptr; }
    virtual bool hasMaterial() const { return false; }
    virtual bool isLight() override { return false; }
    virtual bool isStarsSky() override { return true;  }

    // Mesh
    void setStarsParameters(float radius, int nstars); 
    void initMesh(const std::string& vertexshader, 
                  const std::string& fragshader);

    void setMesh(Mesh* mesh);

    // Rendering
    Mesh* getMesh() const override { return m_mesh; }
    Material* getMaterial() const { return nullptr; }

    // Game 

    // Update 
    void Update(float deltatime) override;


    // Operators
    inline bool operator==(const StarBox* other) {
        return this->getId() == other->getId();
    }

    inline bool operator!=(const StarBox* other) {
        return !(this == other);
    }

    inline bool operator==(const StarBox other) {
        return getId() == other.getId();
    }

    inline bool operator!=(const StarBox other) {
        return !(*this == other);
    }

};

