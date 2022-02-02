#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal; 
layout(location = 2) in vec2 aTextureCoord; 

uniform mat4 u_avp;
uniform mat4 u_atransform;
uniform mat4 u_Ntransform;
uniform vec4 u_acolor;

out vec4 vColor; 
out vec2 vTextureCoord;
out vec3 vNormal; 
out vec4 FragPos; 

void main()
{
    vColor = u_acolor;
    vTextureCoord = aTextureCoord;
    // Normal
    vNormal = vec3(u_Ntransform * vec4(aNormal, 0.0));
    // Position 
    gl_Position = u_avp * u_atransform * vec4(aPos, 1.0);
    // Fragment Pos 
    FragPos = u_atransform * vec4(aPos, 1.0);
    
}
