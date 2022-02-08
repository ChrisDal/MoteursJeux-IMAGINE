#version 330 core

out vec4 FragColor;

// store the material properties of the surface
struct ObjMaterial {
    vec3 ambient; 
    vec3 diffuse;
    vec3 specular; 
    float shininess; 
};

// store the material properties of the Light
struct Light {
    vec4 position;
    vec4 color; 
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

in vec4 vColor; 
in vec2 vTextureCoord;
in vec3 vNormal; 
in vec4 FragPos; 

uniform vec4 u_viewPos; 
uniform ObjMaterial u_material; 
uniform Light u_light;

void main()
{
    // normal 
    vec3 norml = normalize(vNormal);
    // Frag => To light 
    vec3 lightDir = normalize(vec3(u_light.position - FragPos));
    
    // Ambient
    vec4 ka = vec4(u_material.ambient, 1.0f);
    vec4 ambient = ka * u_light.ambient;

    
    // Diffuse
    vec4  kd = vec4(u_material.diffuse, 1.0f);
    float diff = max(dot(norml, lightDir), 0.0);
    vec4 diffuse = kd * diff * u_light.diffuse;


    // Specular 
    vec4 ks = vec4(u_material.specular, 1.0f);
    int shininess = 32; 
    vec3 viewDir = normalize(vec3(u_viewPos - FragPos)); // camera pos
    vec3 reflectDir = normalize(reflect(-lightDir, norml)); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess * 128 );
    vec4 specular = ks * spec * u_light.specular;

    // Total
    FragColor = (ambient + diffuse + specular) ;
    //FragColor = vec4(norml, 1.0);
    
}
