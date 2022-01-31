#version 330 core
out vec4 FragColor;


in vec4 vColor; 
in vec2 vTextureCoord;


//uniform vec3 lightColor;

void main()
{
    //FragColor = vec4(lightColor, 1.0f) *  vColor ;
    FragColor =  vColor ;
    
}
