#version 330 core
out vec4 FragColor;


in vec4 vColor; 
in vec2 vTextureCoord;
in vec3 vNormal; 
in vec4 FragPos; 


uniform vec4 u_lightColor;
uniform vec4 u_lightPos; 

void main()
{
    // normal 
    vec3 norml = normalize(vNormal);
    vec3 lightDir = normalize(vec3(u_lightPos - FragPos));
    
    // Ambient
    float ka = 0.5; 
    vec4 ambient = ka * u_lightColor; 

    
    // Diffuse
    float diff = max(dot(norml, lightDir), 0.0);
    vec4 diffuse = diff * u_lightColor;

    // Total
    FragColor = (ambient + diffuse) *  vColor ;
    
}
