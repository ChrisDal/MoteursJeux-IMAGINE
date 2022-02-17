#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexNA;

uniform mat4 u_viewproj;
uniform mat4 u_transform;
//uniform mat4 u_Ntransform;

out vec4 vColor;
//out vec4 FragPos;

void main()
{
    vColor = vec4(aColor, 1.0f);
    // Position 
    gl_Position = u_viewproj * u_transform * vec4(aPos, 1.0);
    // Fragment Pos 
    //FragPos = u_transform * vec4(aPos, 1.0);

}
