#version 330 core  
out vec4 FragColor;  

in vec2 Texcoord;  

uniform sampler2D terrainTexture;  

void main()  
{  
    FragColor = texture(terrainTexture, Texcoord); // Используем текстуру  
}  