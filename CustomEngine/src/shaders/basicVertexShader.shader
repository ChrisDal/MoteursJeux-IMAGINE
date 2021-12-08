#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor; 
layout(location = 2) in vec2 aTextureCoord; 

uniform mat4 u_mvp;
uniform mat4 u_transform; 
uniform mat4 u_transformexterne; 
 

out vec3 vColor; 
out vec2 vTextureCoord;

void main()
{
    gl_Position = u_transformexterne * u_mvp * u_transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vColor = aColor; 
    vTextureCoord = aTextureCoord;
}
