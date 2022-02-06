#version 330 core

out vec4 FragColor;

// store the material properties of the surface
struct ObjMaterial {
    vec3 ambient; 
    vec3 diffuse;
    vec3 specular; 
    float shininess; 
};

in vec4 vColor; 
in vec2 vTextureCoord;
in vec3 vNormal; 
in vec4 FragPos; 


uniform vec4 u_lightColor;
uniform vec4 u_lightPos; 
uniform vec4 u_viewPos; 
uniform ObjMaterial u_material; 

void main()
{
    // normal 
    vec3 norml = normalize(vNormal);
    // Frag => To light 
    vec3 lightDir = normalize(vec3(u_lightPos - FragPos));
    
    // Ambient
    vec4 ka = vec4(u_material.ambient, 1.0f);
    vec4 ambient = ka * u_lightColor; 

    
    // Diffuse
    vec4  kd = vec4(u_material.diffuse, 1.0f);
    float diff = max(dot(norml, lightDir), 0.0);
    vec4 diffuse = kd * diff * u_lightColor;


    // Specular 
    vec4 ks = vec4(u_material.specular, 1.0f);
    int shininess = 32; 
    vec3 viewDir = normalize(vec3(u_viewPos - FragPos)); // camera pos
    vec3 reflectDir = normalize(reflect(-lightDir, norml)); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess * 128 );
    vec4 specular = ks * spec * u_lightColor;

    // Total
    FragColor = (ambient + diffuse + specular) ;
    //FragColor = vec4(norml, 1.0);
    
}
