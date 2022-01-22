#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal; 
layout(location = 2) in vec2 aTextureCoord; 

uniform mat4 u_vp;
uniform mat4 u_transform;
uniform mat4 u_internal_transform;

out vec3 vColor; 
out vec2 vTextureCoord;

void main()
{
    vColor = aNormal;
    vTextureCoord = aTextureCoord;
    // Position 
    gl_Position = u_vp * u_transform * u_internal_transform * vec4(aPos, 1.0f);
    
}
