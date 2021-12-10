#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor; 
layout(location = 2) in vec2 aTextureCoord; 

uniform mat4 u_mvp;
uniform mat4 u_transform_matrix;
uniform mat4 u_worldmatrix;
uniform mat4 u_internal_tsfm_matrix;

out vec3 vColor; 
out vec2 vTextureCoord;

void main()
{
    vColor = aColor;
    vTextureCoord = aTextureCoord;
    // Position 
    gl_Position = mvp_matrix * u_transform_matrix * u_worldmatrix * u_internal_tsfm_matrix * a_position;
    
}
