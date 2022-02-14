#version 330 core

struct LightMat {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};


in vec4 vColor;
in vec2 vTextureCoord;

uniform LightMat u_light; 


out vec4 FragColor;



void main()
{
    FragColor = 0.2 * u_light.ambient +  0.6 * u_light.diffuse + 0.2 * u_light.specular;

}
