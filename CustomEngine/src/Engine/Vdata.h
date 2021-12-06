#pragma once
#include <string>
#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3

struct VertexData
{
    glm::vec3 position;
    glm::vec2 texCoord;
    float texIndex;

    friend bool operator<(const VertexData& a, const VertexData& b)
    {
        return a.position[0] < b.position[0] && a.position[1] < b.position[1] && a.position[2] < b.position[2];
    }
};

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};


struct Texture {
    unsigned int id;
    std::string type;
};