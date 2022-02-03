#version 330 core
out vec4 FragColor;


in vec4 vColor; 
in vec2 vTextureCoord;
in vec3 vNormal; 
in vec4 FragPos; 


uniform vec4 u_lightColor;
uniform vec4 u_lightPos; 
uniform vec4 u_viewPos; 

void main()
{
    // normal 
    vec3 norml = normalize(vNormal);
    vec3 lightDir = normalize(vec3(u_lightPos - FragPos));
    
    // Ambient
    float ka = 0.5; 
    vec4 ambient = ka * u_lightColor; 

    
    // Diffuse
    float kd = max(dot(norml, lightDir), 0.0);
    vec4 diffuse = kd * u_lightColor;


    // Specular 
    float ks = 0.5;
    int shininess = 32; 
    vec3 viewDir = normalize(vec3(u_viewPos - FragPos)); 
    vec3 reflectDir = reflect(-lightDir, norml); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specular = ks * spec * u_lightColor;

    // Total
    FragColor = (ambient + diffuse + specular) *  vColor ;
    //FragColor = vec4(norml, 1.0);
    
}
