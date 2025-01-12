#version 440 core  
out vec4 FragColor;  

in vec2 TexCoords;  

uniform sampler2D grassTexture;  

void main()  
{  
    vec4 texColor = texture(grassTexture, TexCoords);  
    
    if(texColor.a < 0.1)  
        discard;  
    
    FragColor = texColor;  
}  