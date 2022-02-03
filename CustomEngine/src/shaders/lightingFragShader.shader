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
    // Frag => To light 
    vec3 lightDir = normalize(vec3(u_lightPos - FragPos));
    
    // Ambient
    float ka = 0.5; 
    vec4 ambient = ka * u_lightColor; 

    
    // Diffuse
    float kd = 1.0; 
    float diff = max(dot(norml, lightDir), 0.0);
    vec4 diffuse = kd * diff * u_lightColor;


    // Specular 
    float ks = 0.5;
    int shininess = 32; 
    vec3 viewDir = normalize(vec3(u_viewPos - FragPos)); // camera pos
    vec3 reflectDir = normalize(reflect(-lightDir, norml)); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specular = ks * spec * vec4(0.1f, 0.1f, 1.0f, 1.0f); // blue

    // Total
    FragColor = (ambient + diffuse + specular) *  vColor ;
    //FragColor = vec4(norml, 1.0);
    
}
