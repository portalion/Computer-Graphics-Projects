#version 330 core
out vec4 FragColor;

in vec3 LightingColor; 

uniform vec3 u_Color;

void main()
{
   FragColor = vec4(LightingColor * u_Color, 1.0);
}
