#version 330 core

in vec2 vTextureCoord; 


uniform sampler2D sampler; 
uniform vec3 vColor; 

void main()
{
    vec4 textureColor = texture2D(sampler, vTextureCoord); 
    if (textureColor == 0)
    {
        gl_FragColor = vec4(vColor, 1.0); 
    }
    else 
    {
        gl_FragColor = textureColor * vec4(vColor, 1.0); 
    }
}
