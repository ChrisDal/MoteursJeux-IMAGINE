#pragma once
#include <string>
#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec3.hpp> // glm::vec3

/*struct VertexData
{
    glm::vec3 position;
    glm::vec2 texCoord;
    float texIndex;

    friend bool operator<(const VertexData& a, const VertexData& b)
    {
        return a.position[0] < b.position[0] && a.position[1] < b.position[1] && a.position[2] < b.position[2];
    }
};*/

struct VertexData {
    glm::vec3 positions;
    glm::vec3 normals;
    glm::vec2 texcoords;
};


struct TextureStruct {
    unsigned int id;
    std::string type;
};