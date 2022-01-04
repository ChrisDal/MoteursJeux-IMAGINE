#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor; 
layout(location = 2) in vec2 aTextureCoord; 

uniform mat4 u_vp;
uniform mat4 u_transform_matrix;
uniform mat4 u_world_matrix;
uniform mat4 u_internal_tsfm_matrix;

out vec3 vColor; 
out vec2 vTextureCoord;

void main()
{
    vColor = aColor;
    vTextureCoord = aTextureCoord;
    // Position 
    gl_Position = u_vp * u_transform_matrix * u_world_matrix * u_internal_tsfm_matrix * vec4(aPos, 1.0);
    
}
