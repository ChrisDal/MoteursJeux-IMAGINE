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

    float m_constant; 
    float m_linear; 
    float m_quadratic; 
    int type; 
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
    vec3 lightDir; 
    float attenuation; 
    // 
    if (u_light.type == 0) {

        // Directional Light
        lightDir = normalize(-vec3(u_light.position)); // as a direction 
        attenuation = 1.0f;
    }
    else if (u_light.type == 1)
    {
        // PointLight
        lightDir = normalize(vec3(u_light.position - FragPos));
        float distance = length(vec3(u_light.position - FragPos));
        attenuation = 1.0 / (u_light.m_constant + u_light.m_linear * distance
            + u_light.m_quadratic * (distance * distance));
    }
    else
    {
        lightDir = normalize(vec3(u_light.position - FragPos));
        attenuation = 1.0f; 
    }
    /*if (u_light.position.w == 0.0)
    {
        // Directional Light
        lightDir = normalize(-vec3(u_light.position)); // as a direction 
        attenuation = 1.0f; 

    }
    else if (u_light.position.w == 1.0)
    {
        // PointLight
        lightDir = normalize(vec3(u_light.position - FragPos));
        float distance = length(vec3(u_light.position - FragPos)); 
        attenuation = 1.0 / (u_light.m_constant + u_light.m_linear * distance 
            u_light.m_quadratic * (distance * distance))
    }*/
    
    // Ambient
    vec4 ka = vec4(u_material.ambient, 1.0f);
    vec4 ambient = ka * u_light.ambient;

    
    // Diffuse
    vec4  kd = vec4(u_material.diffuse, 1.0f);
    float diff = max(dot(norml, lightDir), 0.0);
    vec4 diffuse = kd * diff * u_light.diffuse;


    // Specular 
    vec4 ks = vec4(u_material.specular, 1.0f);
    vec3 viewDir = normalize(vec3(u_viewPos - FragPos)); // camera pos
    vec3 reflectDir = normalize(reflect(-lightDir, norml)); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess * 128 );
    vec4 specular = ks * spec * u_light.specular;

    // Total
    FragColor = (ambient + diffuse + specular) * attenuation ;
    
}
