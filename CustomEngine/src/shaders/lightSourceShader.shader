#version 330 core

/*struct LightMat {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};*/


in vec4 vColor;
in vec2 vTextureCoord;



out vec4 FragColor;

void main()
{
    FragColor = vColor;

}
