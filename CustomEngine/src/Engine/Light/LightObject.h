#pragma once

#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4


#include "../BasicGameObject.h"
#include "../Mesh.h"



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
    enum class LightType { DIRECTIONAL=0, POINT, SPOT, FLASH };

protected:
    LightType m_type; 
    float m_distance; // covered distance by light -1 = inf 
    // attenuation
    float m_kc, m_kl, m_kq; 
    float m_Fatt; 


    float processAttenuation(float Kc, float Kl, float Kq, float distance); 
    void presetParameters(float distance); 


public:
    //static int g_id;

    LightObject(SceneNode* parent, glm::vec3 center,
        short int textureId = -1, std::string filename = "",
        std::string tag = "Light");
    ~LightObject();


    // Print game object informations
    void print() override;
    bool hasMesh() override { return true; }
    virtual bool hasMaterial() const { return m_mat != nullptr; }
    virtual bool isLight() override { return true; }

    // Mesh
    void initMesh(int typemesh);
    void initMesh(const char* filename);
    bool loadMesh(const std::string& filename);
    void setColor(const glm::vec4& color) { m_mesh->setColor(color); }
    glm::vec4 getColor() const { return m_mesh->getColor(); }

    // Material  
    void initMaterial(Texture* texture, const glm::vec3& color);

    // Rendering
    Mesh* getMesh() const override { return m_mesh; }
    Material* getMaterial() const { return m_mat; }
    void setMaterial(const glm::vec3& amb, 
                    const glm::vec3& diff,
                    const glm::vec3& spec, 
                    const float& shininess);

    // Light Type 
    void setType(LightType type, float distance); 
    void setType(unsigned int type, float distance); 
    unsigned int getIntType() const { return static_cast<unsigned int>(m_type);  }
    LightType getType() const { return m_type;  }
    inline void setParameters(float kc, float kl, float kq); 
    glm::vec3 getParameters() const { return glm::vec3(m_kc, m_kl, m_kq); }

    // Update 
    void Update(float deltatime) override;

    // Operators
    inline bool operator==(const LightObject* other) {
        return this->getId() == other->getId();
    }

    inline bool operator!=(const LightObject* other) {
        return !(this == other);
    }

    inline bool operator==(const LightObject other) {
        return getId() == other.getId();
    }

    inline bool operator!=(const LightObject other) {
        return !(*this == other);
    }


};
