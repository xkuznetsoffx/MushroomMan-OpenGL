#version 440 core
out vec4 color;

uniform vec3 lightColor;

void main()
{
    color = vec4(lightColor, 1.0f); // Устанавливает все 4 компоненты вектора равными 1.0f
}