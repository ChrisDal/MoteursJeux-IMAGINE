#version 330 core
out vec4 FragColor;

uniform sampler2D texture1; 
uniform sampler2D texture2; 

in vec3 vColor; 
in vec2 vTextureCoord;

void main()
{
    FragColor = mix(texture(texture1, vTextureCoord), texture(texture2, vTextureCoord), 0.2) * vec4(vColor, 1.0f);
}
